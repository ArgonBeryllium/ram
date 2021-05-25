#pragma once
#include "operators.h"

struct SDF;
struct Ray
{
	v3f ori, dir;
};
struct Intersection
{
	bool intersecting;
	float min_dist;
	float max_dist;
	v3f normal;
	SDF* sdf;
};
