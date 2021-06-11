#include "sdf.h"

Shader* Shader::def_inst = new Shader{};

static constexpr float SMOOTH = .1, REFL = .9;
static const SDL_Colour GI = {1,1,1,255};
SDL_Colour Shader::getPixelValue(SDF *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	float r, g, b, a = 1;
	r = g = b = .0;

	Ray ray = o_ray;
	ray.ori = ray.ori+ray.dir*(o_i.min_dist-.0001);
	ray.dir = (lp-ray.ori).normalised();
	//r = g = b = (1.f-std::min(1.f, std::max(0.f, (ray.ori-lp).getLengthSquared()/li)));
	
	v3f normal = obj->getNormal(ray.ori);
	r = g = b = std::pow(std::abs(dot(normal, (obj->pos-lp).normalised())), SMOOTH*SMOOTH*1000) / (ray.ori-lp).getLengthSquared()*li;

	for(SDF* obj_ : world)
	{
		Intersection i = obj_->getIntersection(ray);
		if(i.intersecting)
		{
			r = g = b = 0;
			break;
		}
	}

	if(rec<4)
		for(SDF* obj_ : world)
		{
			ray.dir = (ray.ori-obj->pos).normalised();
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
