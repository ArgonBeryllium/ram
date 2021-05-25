#pragma once
#include "operators.h"
#include "helpers.h"

struct Camera
{
	static v3f pos;
	static float fov;
	static float plane_offset;

	static void init();

	static v3f projectPtoS(const v2i& pp, float zd = 0);
	static v3f getRayDir(const v2i& pp);
	static v2i projectStoP(const v3f& pp);
};
