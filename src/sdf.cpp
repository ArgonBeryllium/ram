#include "sdf.h"

Shader* Shader::def_inst = new Shader{};
SDL_Colour Shader::getPixelValue(SDF *obj, Ray ray, Intersection i, uint32_t rec)
{
	float r, g, b, a = 1;
	r = g = b = .0;

	ray.ori = ray.ori+ray.dir*(i.min_dist-.01);
	ray.dir = (lp-ray.ori).normalised();
	r = g = b = (1.f-std::min(1.f, std::max(0.f, (ray.ori-lp).getLengthSquared()/li)));
	for(SDF* obj_ : world)
	{
		i = obj_->getIntersection(ray);
		if(i.intersecting)
		{
			if(rec>4)
			{
				r = g = b = 0;
				break;
			}
			SDL_Colour nc = obj_->shader->getPixelValue(obj_, ray, i, rec+1);
			constexpr float RC = .4;
			r = (r+RC*float(nc.r)/255)/(1+RC);
			g = (g+RC*float(nc.g)/255)/(1+RC);
			b = (b+RC*float(nc.b)/255)/(1+RC);
		}
	}
	i = obj->getIntersection(ray);
	return SDL_Colour{Uint8(r*255), Uint8(g*255), Uint8(b*255), Uint8(a*255)};
}
