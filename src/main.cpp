#include <cmath>
#include <shitrndr.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <limits.h>

#include "operators.h"
#include "helpers.h"
#include "globals.h"
#include "camera.h"
#include "ray.h"
#include "sdf.h"
using namespace shitrndr;

const static size_t num_samples = 1;

SDL_Colour castRay(int x, int y, int seed = -1)
{
	if(seed!=-1) std::srand(seed);
	SDL_Colour out = {0,0,0,255};

	Ray ray = {Camera::projectPtoS({x,y}), Camera::getRayDir({x,y})};
	v2i ro = getHelperCoords(ray.ori);
	v2i re = ro+(getHelperDir(ray.dir)*512).to<int>();
	SetRenderColour(oren, {255,255,255,1});

	float sd = MAXFLOAT;
	for(SDF* obj : world)
	{
		Intersection i = obj->getIntersection(ray);

		SetRenderColour(oren, {255,255,255,1});

		if(i.intersecting && i.min_dist>0 && i.min_dist<sd)
		{
			sd = i.min_dist;
			out = obj->shader->getPixelValue(obj, ray, i);

			SetRenderColour(oren, {255,0,0,1});
		}

		SDL_RenderDrawLine(oren, ro.x, ro.y, re.x, re.y);
	}
	SetRenderColour(oren, {255,255,255,255});

	return out;
}
SDL_Colour getPixelColour(int x, int y)
{
	MegaCol out;
	for (size_t i = 0; i != num_samples; i++)
		out += castRay(x, y, x+y*2);
	out /= num_samples;
	return out.col();
}

v3f iv;
int main()
{
	// rendering setup
	{
		init(".", RW, RH, 1, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		WindowProps::setPixScale(PS);
		WindowProps::lock_type = WindowProps::BARS;
		WindowProps::setLocked(1);
		silentDefs();
		initHelpers();

		bg_col = {0,0,0,0};
	}

	world.push_back(new Sphere());
	world.push_back(new Sphere({0,-5,0}, 3));
	world.push_back(new Sphere({0,-3,4}, 3));
	world.push_back(new Plane({0,-3,0}));

	onRender = [](double delta, double time)
	{
		SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
		SDL_RenderClear(oren);
		SetRenderColour(oren, {255,255,255,255});

		lp.x = std::cos(time)*5;
		lp.z = std::sin(time)*5;

		// camera controls
		{
			iv = {((gk(SDLK_d) || gk(SDLK_RIGHT))?1.f:0.f) - ((gk(SDLK_a) || gk(SDLK_LEFT))?1.f:0.f),
					((gk(SDLK_w) || gk(SDLK_UP))?1.f:0.f) - ((gk(SDLK_s) || gk(SDLK_DOWN))?1.f:0.f)};
			Camera::pos += v3f{iv.x, Input::getKey(SDLK_LSHIFT)?iv.y:0, Input::getKey(SDLK_LSHIFT)?0:iv.y}*3*delta;

			if(Input::getKey(SDLK_q)) Camera::angles.y += delta*2;
			if(Input::getKey(SDLK_e)) Camera::angles.y -= delta*2;
		}

		// main loop
		for (int y = 0; y != H; y++)
			for (int x = 0; x != W; x++)
			{
				SDL_Colour col = getPixelColour(x, y);
				if(col.a==0) continue;
				SetColour(col);
				SDL_RenderDrawPoint(ren, x, y);
			}
		SetColour({0,0,255});

		renderFPS(time, delta);

		// debug window
		{
			v2i cp = getHelperCoords(Camera::pos);
			RenderFillCircle(oren, cp.x, cp.y, 2);
			for(SDF* obj : world)
				obj->renderPreview(oren);
			v2i slp = getHelperCoords(lp);
			RenderFillCircle(oren, slp.x, slp.y, 2);
		}

		if(Input::getKey(SDLK_q)) std::exit(0);
		SDL_RenderPresent(oren);
	};

	loop();
	return 0;
}
