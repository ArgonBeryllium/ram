#include "sdf.h"

Shader* Shader::def_inst = new Shader_Def{};

static constexpr size_t BOUNCE_COUNT = 4;
SDL_Colour Shader_Def::getPixelValue(SDF *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	FloatCol out = {0,0,0,1};
	float& r = out.r;
	float& g = out.g;
	float& b = out.b;
	float& a = out.a;

	v3f point_surf = o_ray.ori+o_ray.dir*(o_i.min_dist-.0001);
	v3f normal = obj->getNormal(point_surf);
	v3f dir_rough_offset = v3f{frand(), frand(), frand()}*.1*(1-smooth);

	Ray ray = o_ray;
	ray.ori = point_surf;
	
	for(Light* l : lights)
	{
		v3f dir_light = (l->pos-point_surf).normalised() + dir_rough_offset;

		ray.dir = dir_light;

		bool abstructed = 0;
		for(SDF* obj_ : world)
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
			r += lum*l->col.r;
			g += lum*l->col.g;
			b += lum*l->col.b;
		}
	}

	if(rec<BOUNCE_COUNT)
		for(SDF* obj_ : world)
		{
			ray.dir = normal + dir_rough_offset;
			Intersection i = obj_->getIntersection(ray);
			if(i.intersecting)
			{
				SDL_Colour nc = obj_->shader->getPixelValue(obj_, ray, i, rec+1);
				r += reflective*float(nc.r)/255;
				g += reflective*float(nc.g)/255;
				b += reflective*float(nc.b)/255;
			}
		}

	r *= col.r;
	g *= col.g;
	b *= col.b;
	a *= col.a;

	r += GI.r;
	g += GI.g;
	b += GI.b;

	r = std::max(0.f, std::min(1.f, r));
	g = std::max(0.f, std::min(1.f, g));
	b = std::max(0.f, std::min(1.f, b));
	a = std::max(0.f, std::min(1.f, a));

	return out.col();
}
