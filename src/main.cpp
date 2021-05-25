#include <shitrndr.h>
#include <SDL2/SDL_ttf.h>

#include "operators.h"
#include "helpers.h"
#include "globals.h"
#include "camera.h"
#include "ray.h"
#include "sdf.h"
using namespace shitrndr;

const static size_t num_samples = 1;
Sphere sph = {};
SDL_Colour castRay(int x, int y, int seed = -1)
{
	if(seed!=-1) std::srand(seed);
	float r, g, b, a = 1;
	r = float(x)/W/2;
	b = float(y)/H/2;

	Ray ray = {Camera::projectPtoS({x,y}), Camera::getRayDir({x,y})};
	v2i ro = getHelperCoords(ray.ori);
	v2i re = Input::getKey(SDLK_LSHIFT) ? ro+(v2f{ray.dir.x, -ray.dir.z}*512).to<int>() : ro+(v2f{ray.dir.z, -ray.dir.y}*512).to<int>();
	SetRenderColour(oren, {255,255,255,1});

	Intersection i = sph.getIntersection(ray);
	if(i.intersecting && i.min_dist>0)
	{
		r = g = b = 1;
		SetRenderColour(oren, {255,0,0,1});
	}
	SDL_RenderDrawLine(oren, ro.x, ro.y, re.x, re.y);
	SetRenderColour(oren, {255,255,255,255});

	return {Uint8(r*255), Uint8(g*255), Uint8(b*255), Uint8(a*255)};
}
SDL_Colour getPixelColour(int x, int y)
{
	MegaCol out;
	for (size_t i = 0; i != num_samples; i++)
		out += castRay(x, y);
	out /= num_samples;
	return out.col();
}

v3f iv;
int main()
{
	init(".", RW, RH, 1, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	WindowProps::setPixScale(PS);
	WindowProps::lock_type = WindowProps::BARS;
	WindowProps::setLocked(1);
	silentDefs();
	initHelpers();

	SDL_SetRenderDrawBlendMode(oren, SDL_BLENDMODE_BLEND);

	bg_col = {0,0,0,0};
	onRender = [](double delta, double time)
	{
		Camera::fov = std::abs(std::sin(time))*M_PIf32;
		SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
		SDL_RenderClear(oren);
		SetRenderColour(oren, {255,255,255,255});

		iv = {((gk(SDLK_d) || gk(SDLK_RIGHT))?1.f:0.f) - ((gk(SDLK_a) || gk(SDLK_LEFT))?1.f:0.f),
				((gk(SDLK_w) || gk(SDLK_UP))?1.f:0.f) - ((gk(SDLK_s) || gk(SDLK_DOWN))?1.f:0.f)};
		Camera::pos += v3f{iv.x, 0, iv.y}*3*delta;

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

		{
			v2i cp = getHelperCoords(Camera::pos);
			RenderFillCircle(oren, cp.x, cp.y, 2);
			v2i sp = getHelperCoords(sph.pos);
			RenderFillCircle(oren, sp.x, sp.y, 8);
			renderText(std::to_string(Camera::fov), 100, 15);
		}
		SDL_RenderPresent(oren);

		if(Input::getKey(SDLK_q))
		{
			SDL_Quit();
			std::exit(0);
		}
	};

	loop();
	return 0;
}
