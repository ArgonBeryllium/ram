#include <shitrndr.h>
#include <vector>
#include <limits.h>

#define DEBUG_WINDOW
#include "operators.h"
#include "helpers.h"
#include "camera.h"
#include "ray.h"
#include "solids.h"
#include "world.h"
using namespace shitrndr;

const static size_t num_samples = 1;
constexpr int RW = 540, RH = 480, PS=5,
						W = RW/PS, H = RH/PS;
World world;


SDL_Colour getPixelColour(int x, int y)
{
	MegaCol out;
	for (size_t i = 0; i != num_samples; i++)
		out += world.cam->castRay(x, y, x+y*W);
	out /= num_samples;
	return out.col();
}

v3f iv;
int main()
{
	// rendering setup
	init(".", RW, RH, 1, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	WindowProps::setPixScale(PS);
	WindowProps::lock_type = WindowProps::BARS;
	WindowProps::setLocked(1);
	silentDefs();
	bg_col = {0,0,0,0};

#ifdef DEBUG_WINDOW
	initHelpers();
#endif

	Shader_Def red_n_shiny = Shader_Def();
	red_n_shiny.col = {1,0,0,1};
	red_n_shiny.smooth = .8;
	red_n_shiny.reflective = .9;

	world.solids.push_back(new Sphere(&world));
	world.solids[0]->shader  = &red_n_shiny;

	world.solids.push_back(new Sphere(&world, {0,-5,0}, 3));
	world.solids.push_back(new Sphere(&world, {0,-3,4}, 3));
	world.solids.push_back(new Plane(&world, {0,-3,0}));

	world.lights.push_back(new Light{{}, 10});
	world.lights.push_back(new Light{{-20, 20, -4}, 400.f, {1, .9, .8}});

	onRender = [](double delta, double time)
	{
#ifdef DEBUG_WINDOW
		SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
		SDL_RenderClear(oren);
		SetRenderColour(oren, {255,255,255,255});
#endif

		world.lights[0]->col.r = std::abs(std::sin(time))*10;
		world.lights[0]->col.g = std::abs(std::cos(time))*10;
		world.lights[0]->col.b = std::abs(std::sin(time+1))*10;

		world.lights[0]->pos.x = std::cos(time)*5;
		world.lights[0]->pos.y = std::cos(time)*2 + 1;
		world.lights[0]->pos.z = std::sin(time)*5;

		world.solids[0]->pos.y = std::sin(time+.2);

		// camera controls
		{
			iv = {((gk(SDLK_d) || gk(SDLK_RIGHT))?1.f:0.f) - ((gk(SDLK_a) || gk(SDLK_LEFT))?1.f:0.f),
					((gk(SDLK_w) || gk(SDLK_UP))?1.f:0.f) - ((gk(SDLK_s) || gk(SDLK_DOWN))?1.f:0.f)};
			world.cam->pos += v3f{iv.x, Input::getKey(SDLK_LSHIFT)?iv.y:0, Input::getKey(SDLK_LSHIFT)?0:iv.y}*3*delta;

			if(Input::getKey(SDLK_j)) world.cam->angles.y -= delta*2;
			if(Input::getKey(SDLK_l)) world.cam->angles.y += delta*2;
			if(Input::getKey(SDLK_i)) world.cam->angles.x -= delta*2;
			if(Input::getKey(SDLK_k)) world.cam->angles.x += delta*2;
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

#ifdef DEBUG_WINDOW
		renderFPS(time, delta);

		v2i cp = getHelperCoords(world.cam->pos);
		RenderFillCircle(oren, cp.x, cp.y, 2);
		for(Solid* obj : world.solids)
			obj->renderPreview(oren);

		v2i slp = getHelperCoords(world.lights[0]->pos);
		RenderFillCircle(oren, slp.x, slp.y, 2);

		SDL_RenderPresent(oren);
#endif

		if(Input::getKey(SDLK_q)) std::exit(0);
	};

	loop();
	return 0;
}
