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
#include "solids.h"
using namespace shitrndr;

const static size_t num_samples = 1;

SDL_Colour castRay(int x, int y, int seed = -1)
{
	if(seed!=-1) std::srand(seed);
	SDL_Colour out = GI.col();

	Ray ray = {Camera::getPlaneCoord({x,y}), Camera::getRayDir({x,y})};
	v2i ro = getHelperCoords(ray.ori);
	v2i re = ro+(getHelperDir(ray.dir)*512).to<int>();
	SetRenderColour(oren, {255,255,255,1});

	float sd = MAXFLOAT;
	for(Solid* obj : world)
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
		out += castRay(x, y, x+y*W);
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

	Shader_Def red_n_shiny = Shader_Def();
	red_n_shiny.col = {1,0,0,1};
	red_n_shiny.smooth = .8;
	red_n_shiny.reflective = .9;

	world.push_back(new Sphere());
	world[world.size()-1]->shader  = &red_n_shiny;

	world.push_back(new Sphere({0,-5,0}, 3));
	world.push_back(new Sphere({0,-3,4}, 3));
	world.push_back(new Plane({0,-3,0}));

	lights.push_back(new Light{{}, 10});
	lights.push_back(new Light{{-20, 20, -4}, 400.f, {1, .9, .8}});

	onRender = [](double delta, double time)
	{
		SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
		SDL_RenderClear(oren);
		SetRenderColour(oren, {255,255,255,255});

		lights[0]->col.r = std::abs(std::sin(time))*10;
		lights[0]->col.g = std::abs(std::cos(time))*10;
		lights[0]->col.b = std::abs(std::sin(time+1))*10;

		lights[0]->pos.x = std::cos(time)*5;
		lights[0]->pos.y = std::cos(time)*2 + 1;
		lights[0]->pos.z = std::sin(time)*5;

		world[0]->pos.y = std::sin(time+.2);

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

		renderFPS(time, delta);

		// debug window
		{
			v2i cp = getHelperCoords(Camera::pos);
			RenderFillCircle(oren, cp.x, cp.y, 2);
			for(Solid* obj : world)
				obj->renderPreview(oren);
			v2i slp = getHelperCoords(lights[0]->pos);
			RenderFillCircle(oren, slp.x, slp.y, 2);
		}

		if(Input::getKey(SDLK_q)) std::exit(0);
		SDL_RenderPresent(oren);
	};

	loop();
	return 0;
}
