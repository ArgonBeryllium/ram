#pragma once
#include "operators.h"

struct Solid;
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
	Solid* solid;
};
