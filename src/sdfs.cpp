#include "sdfs.h"
#include "world.h"
#include "ray.h"

Shader* Shader::def_inst = new Shader_Def{};
static constexpr size_t BOUNCE_COUNT = 3;

FloatCol Shader_Def::getPixelValue(SDF *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	FloatCol out = {0,0,0,1};

	#ifdef RAM_DEBUG
	if(rec==0)
	{
		SDL_SetRenderDrawColor(oren, 0, 250, 255, 255);
		v2i pp = getHelperCoords(o_i.surf_point);
		SDL_RenderDrawPoint(oren, pp.x, pp.y);
	}
	#endif

	v3f normal = obj->getNormal(o_i.surf_point);
	v3f dir_rough_offset = v3f{frand(), frand(), frand()}*.1*(1-smooth);

	Ray ray = o_ray;
	ray.ori = o_i.surf_point;
	
	// lighting
	for(Light* l : obj->parent_world->lights)
	{
		v3f dir_light = (l->pos-o_i.surf_point).normalised();
		ray.dir = dir_light;
		if(!ray.march(obj->parent_world).intersecting)
		{
			float lum = std::max(1-(std::max(o_i.steps-1.f,0.f)*ambient_occlusion), 0.f) * 5 * // AO
			            std::pow(std::abs(dot(normal, dir_light.normalised())), std::pow(smooth, 5.)*1000) / (o_i.surf_point-l->pos).getLengthSquared()*l->intensity;
			out += l->col*lum;
		}
	}

	// reflections
	if(rec<BOUNCE_COUNT)
	{
		ray.dir = normal + dir_rough_offset;
		Intersection i = ray.march(obj->parent_world, {obj});
		if(i.intersecting)
		{
			FloatCol nc = i.sdf->shader->getPixelValue(i.sdf, ray, i, rec+1);
			out += nc*reflective;
		}
	}

	out *= col;
	out += obj->parent_world->global_illum;

	out.r = std::max(0.f, std::min(1.f, out.r));
	out.g = std::max(0.f, std::min(1.f, out.g));
	out.b = std::max(0.f, std::min(1.f, out.b));
	out.a = std::max(0.f, std::min(1.f, out.a));

	return out;
}
