#pragma once
#include <vector>
#include "camera.h"
#include "colours.h"
#include "solids.h"

struct World
{
	Camera* cam = new Camera{this};
	FloatCol global_illum = {.1, .1, .2};

	std::vector<Solid*> solids;
	std::vector<Light*> lights;

	~World()
	{
		delete cam;
		for(Solid* obj : solids) delete obj;
		for(Light* light : lights) delete light;
	}
};
