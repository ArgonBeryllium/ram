#include "sdf.h"

Shader* Shader::def_inst = new Shader{};

static constexpr float SMOOTH = .3, REFL = .05;
static constexpr size_t BOUNCE_COUNT = 2;
SDL_Colour Shader::getPixelValue(SDF *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	float r, g, b, a = 1;
	r = g = b = .0;

	v3f point_surf = o_ray.ori+o_ray.dir*(o_i.min_dist-.0001);
	v3f dir_light = (lp-point_surf).normalised();
	//r = g = b = (1.f-std::min(1.f, std::max(0.f, (ray.ori-lp).getLengthSquared()/li)));
	
	v3f normal = obj->getNormal(point_surf);
	r = g = b = std::pow(std::abs(dot(normal, (point_surf-lp).normalised())), std::pow(SMOOTH, 5.)*1000) / (point_surf-lp).getLengthSquared()*li;

	Ray ray = o_ray;
	ray.ori = point_surf;
	ray.dir = dir_light;

	for(SDF* obj_ : world)
	{
		Intersection i = obj_->getIntersection(ray);
		if(i.intersecting)
		{
			r = g = b = 0;
			break;
		}
	}

	if(rec<BOUNCE_COUNT)
		for(SDF* obj_ : world)
		{
			ray.dir = (ray.ori-obj->pos).normalised() + v3f{frand(), frand(), frand()}*.1*(1-SMOOTH);
			Intersection i = obj_->getIntersection(ray);
			if(i.intersecting)
			{
				SDL_Colour nc = obj_->shader->getPixelValue(obj_, ray, i, rec+1);
				r += REFL*float(nc.r)/255;
				g += REFL*float(nc.g)/255;
				b += REFL*float(nc.b)/255;
			}
		}
	r += float(GI.r)/255;
	g += float(GI.g)/255;
	b += float(GI.b)/255;
	return SDL_Colour{Uint8(std::min(1.f, std::max(0.f, r))*255), Uint8(std::min(1.f, std::max(0.f, r))*255), Uint8(std::min(1.f, std::max(0.f, b))*255), Uint8(std::min(1.f, std::max(0.f, a))*255)};
}
