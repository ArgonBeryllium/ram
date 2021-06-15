#pragma once
#include "operators.h"
#include "helpers.h"

struct Camera
{
	static v3f pos;
	static v3f angles;
	static float fov;
	static float plane_offset;

	static void init();

	static v3f getPlaneCoord(const v2i& pp);
	static v3f getRayDir(const v2i& pp);
};
