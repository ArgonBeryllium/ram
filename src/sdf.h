#pragma once
#include "operators.h"
#include "ray.h"

struct SDF
{
	v3f pos;
	virtual Intersection getIntersection(Ray ray) = 0;
};
struct Sphere : SDF
{
	float radius = 1;
	Intersection getIntersection(Ray ray)
	{	
		v3f od = ray.ori-pos;
		float b = dot(ray.dir, od);
		float fac = dot(od, od) - radius*radius;
		float d = b*b - fac;
		if(d<0) return {0};
    
		float d1, d2;
		d1 = -b + std::sqrt(d);
		d2 = -b - std::sqrt(d);
		float min_dist = std::min(d1, d2);
		float max_dist = std::max(d1, d2);
		return Intersection{min_dist>0, min_dist, max_dist, (ray.ori+ray.dir*min_dist-pos).normalised(), this};
	};
};
