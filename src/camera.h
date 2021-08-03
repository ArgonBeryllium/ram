#pragma once
#include "operators.h"
#include "helpers.h"
#include "vec3.h"
#include "colours.h"

struct World;
struct Camera
{
	v3f pos = {0,0,-5};
	v3f angles = {};
	
	float fov = M_PIf32/4;
	float plane_offset = .5;
	
	v2i output_dims = {108, 96};
	
	World* parent_world;
	
	Camera(World* parent_world_) : parent_world(parent_world_) {}
	
	void renderFrame(SDL_Rect* rect = 0, size_t samples = 1, SDL_Renderer* target = shitrndr::ren);
	
	v2f getPlaneDims();
	v3f getPlaneCoord(const v2i& pp);
	v3f getRayDir(const v2i& pp);
	
	FloatCol castRay(int x, int y, int seed);

	inline v3f forward() { return rotatePoint({0,0,1}, angles); }
	inline v3f back() { return forward()*-1; }
	inline v3f right() { return rotatePoint({1,0,0}, angles); }
	inline v3f left() { return right()*-1; }
	inline v3f up() { return rotatePoint({0,1,0}, angles); }
	inline v3f down() { return up()*-1; }
};
