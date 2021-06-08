#include "sdf.h"

Shader* Shader::def_inst = new Shader{};
SDL_Colour Shader::getPixelValue(SDF *obj, const Ray& o_ray, const Intersection& o_i, uint32_t rec)
{
	float r, g, b, a = 1;
	r = g = b = .0;

	Ray ray = o_ray;
	ray.ori = ray.ori+ray.dir*(o_i.min_dist-.001);
	ray.dir = (lp-ray.ori).normalised();
	//r = g = b = (1.f-std::min(1.f, std::max(0.f, (ray.ori-lp).getLengthSquared()/li)));
	
	// TODO implement these in the actual SDFs
	v3f normal = (obj->pos-ray.ori).normalised();
	r = g = b = std::max(0.f, std::min(1.f, std::pow(dot(normal, (obj->pos-lp).normalised()), 10.f)));

	for(SDF* obj_ : world)
	{
		Intersection i = obj_->getIntersection(ray);
		if(i.intersecting)
		{
			r = g = b = 0;
			break;
		}
	}
	ray.dir = (ray.ori-obj->pos).normalised();

	if(rec<9)
		for(SDF* obj_ : world)
		{
			Intersection i = obj_->getIntersection(ray);
			if(i.intersecting)
			{
				SDL_Colour nc = obj_->shader->getPixelValue(obj_, ray, i, rec+1);
				constexpr float RC = .5;
				r = (r*(1-RC)+RC*float(nc.r)/255);
				g = (g*(1-RC)+RC*float(nc.g)/255);
				b = (b*(1-RC)+RC*float(nc.b)/255);
			}
		}
	return SDL_Colour{Uint8(r*255), Uint8(g*255), Uint8(b*255), Uint8(a*255)};
}
