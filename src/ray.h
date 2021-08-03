#pragma once
#include "operators.h"
#include "vec3.h"
#include "sdfs.h"
#include "world.h"
#include <vector>

struct Intersection
{
	bool intersecting;
	float min_dist;
	float max_dist;
	v3f normal, surf_point;
	SDF* sdf;
};
static constexpr float DIST_MIN_THRESHOLD = .0001, DIST_MAX_THRESHOLD = 1000;
struct Ray
{
	v3f ori, dir;

	Intersection march(World* world, std::vector<SDF*> exclude = {})
	{
		std::vector<SDF*> sdfs;
		if(!exclude.empty())
		{
			for(SDF* sdf : world->sdfs)
			{
				for(auto i = exclude.begin(); i < exclude.end(); i++)
					if(sdf==(*i.base()))
					{
						exclude.erase(i);
						goto cnt;
					}
				sdfs.push_back(sdf);
				cnt:;
			}
		}
		else
			sdfs = world->sdfs;
		float min_dist = MAXFLOAT, max_dist = 0;
		float d = 0, md = 0;
		v3f p = ori;
		while(d<DIST_MAX_THRESHOLD)
		{
			p = p+dir*md;
			md = MAXFLOAT;
			for(SDF* sdf : world->sdfs)
			{
				d = sdf->getSurfaceDist(p);
				md = std::min(md, d);
				if(d<min_dist)
				{
					min_dist = d;
					if(d<DIST_MIN_THRESHOLD)
					{
						v3f n = sdf->getNormal(p);
						return Intersection{1, min_dist, max_dist, n, p+n*.0001, sdf};
					}
				}
				max_dist = std::max(d, max_dist);
			}
		}
		return Intersection{0, min_dist, max_dist};
	}
};
