#include "solids.h"
#include "world.h"

Shader* Shader::def_inst = new Shader_Def{};

static constexpr size_t BOUNCE_COUNT = 4;
FloatCol Shader_Def::getPixelValue(Solid *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	FloatCol out = {0,0,0,1};

	v3f point_surf = o_ray.ori+o_ray.dir*(o_i.min_dist-.0001);
	v3f normal = obj->getNormal(point_surf);
	v3f dir_rough_offset = v3f{frand(), frand(), frand()}*.1*(1-smooth);

	Ray ray = o_ray;
	ray.ori = point_surf;
	
	// lighting
	for(Light* l : obj->parent_world->lights)
	{
		v3f dir_light = (l->pos-point_surf).normalised() + dir_rough_offset;

		ray.dir = dir_light;

		bool abstructed = 0;
		for(Solid* obj_ : obj->parent_world->solids)
		{
			Intersection i = obj_->getIntersection(ray);
			if(i.intersecting)
			{
				abstructed = 1;
				break;
			}
		}
		if(!abstructed)
		{
			float lum = std::pow(std::abs(dot(normal, dir_light.normalised())), std::pow(smooth, 5.)*1000) / (point_surf-l->pos).getLengthSquared()*l->intensity;
			out += l->col*lum;
		}
	}

	// reflections
	if(rec<BOUNCE_COUNT)
		for(Solid* obj_ : obj->parent_world->solids)
		{
			ray.dir = normal + dir_rough_offset;
			Intersection i = obj_->getIntersection(ray);
			if(i.intersecting)
			{
				FloatCol nc = obj_->shader->getPixelValue(obj_, ray, i, rec+1);
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
