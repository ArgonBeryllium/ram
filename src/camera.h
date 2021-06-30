#pragma once
#include "operators.h"
#include "helpers.h"

struct World;
struct Camera
{
	v3f pos = {0,0,-5};
	v3f angles = {};

	float fov = M_PIf32/4;
	float plane_offset = .2;

	v2i output_dims = {108, 96};

	World* parent_world;

	Camera(World* parent_world_) : parent_world(parent_world_) {}

	v2f getPlaneDims();
	v3f getPlaneCoord(const v2i& pp);
	v3f getRayDir(const v2i& pp);

	SDL_Colour castRay(int x, int y, int seed);
};
