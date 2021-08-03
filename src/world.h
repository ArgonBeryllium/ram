#pragma once
#include <vector>
#include "camera.h"
#include "colours.h"
#include "sdfs.h"

struct World
{
	Camera* cam = new Camera{this};
	FloatCol global_illum = {.1, .1, .2};

	std::vector<SDF*> sdfs;
	std::vector<Light*> lights;

	~World()
	{
		delete cam;
		for(SDF* obj : sdfs) delete obj;
		for(Light* light : lights) delete light;
	}
};
