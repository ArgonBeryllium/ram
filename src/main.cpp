#include <shitrndr.h>
#include <SDL2/SDL_ttf.h>
#include "operators.h"
#include "helpers.h"
using namespace shitrndr;

constexpr int RW = 540, RH = 480, PS=4,
				W = RW/PS, H = RH/PS;
const static size_t num_samples = 1;
constexpr float PPU=20, FOV = .5;

const static v3f light_p = {0,1,-1};

struct Ray
{
	v3f ori, dir;
};
struct Sphere
{
	v3f pos;
	float r = 1;
};
Sphere sph = {{}, 1};

v3f pp{0,0,-5};
v2f iv{};

int getRayLength(const Ray& r, const Sphere& s, float* d1, float* d2)
{
	v3f od = r.ori-s.pos;
	float b = dot(r.dir, od);
	float fac = dot(od, od) - s.r*s.r;
	float d = b*b - fac;
	if(d<0) return 0;

	*d1 = -b + std::sqrt(d);
	*d2 = -b - std::sqrt(d);
	return *d1==*d2?1:2;
}

SDL_Colour castRay(int x, int y, int seed = -1)
{
	if(seed!=-1) std::srand(seed);
	float r, g, b, a = 1;
	r = float(x)/W/2;
	g = frand()*.1;
	b = float(y)/H/2;

	v3f dir = v3f{float(x-W/2)/(W/2)*FOV,float(y-H/2)/(H/2)*FOV,1};
	dir = dir.normalised();
	dir.z = std::cos(dir.x);
	//Ray ray{pp+v3f{(x*2.f-W)/PPU, (y*2.f-H)/PPU, 0}, dir};
	Ray ray{pp, dir};
	float d1, d2;
	if(getRayLength(ray, sph, &d1, &d2))
	{
		float d = d1<d2?d1:d2;
		if(d<0) goto returnlabel;
			helpers::vec2<int> spp = {int(pp.x*8)+128, 8-int(pp.z*8)+128};
			v2f pr = v2f(ray.dir.x, -ray.dir.z);
			SDL_SetRenderDrawColor(oren, 255,0,0,255);
			SDL_RenderDrawLine(oren, spp.x, spp.y, int(spp.x+pr.x*80), int(spp.y+pr.y*80));
		r = g = b = 1;
		ray.ori = ray.ori+ray.dir*d*.9f;
		ray.dir = (light_p-ray.ori).normalised()*-1;
		if(getRayLength(ray, sph, &d1, &d2))
		{
			float d = d1<d2?d1:d2;
			r = g = b = d;
		}
	}
returnlabel:
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

int main()
{
	init(".", RW, RH, 1, 0, SDL_RENDERER_PRESENTVSYNC);
	WindowProps::setPixScale(PS);
	WindowProps::lock_type = WindowProps::BARS;
	WindowProps::setLocked(1);
	silentDefs();
	initHelpers();

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	bg_col = {0,0,0,0};
	onRender = [](double delta, double time)
	{
		SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
		SDL_RenderClear(oren);
		SetRenderColour(oren, {255,255,255,255});

		iv = {((gk(SDLK_d) || gk(SDLK_RIGHT))?1.f:0.f) - ((gk(SDLK_a) || gk(SDLK_LEFT))?1.f:0.f),
				((gk(SDLK_w) || gk(SDLK_UP))?1.f:0.f) - ((gk(SDLK_s) || gk(SDLK_DOWN))?1.f:0.f)};
		pp += v3f{iv.x, 0, iv.y}*3*delta;

		/*
		sph.pos.x = std::sin(time)*3;
		sph.pos.z = std::cos(time)*3;
		sph.pos.y = std::sin(time)*4;
		*/
		for (int y = 0; y != H; y++)
			for (int x = 0; x != W; x++)
			{
				SDL_Colour col = getPixelColour(x, y);
				if(col.a==0) continue;
				SetColour(col);
				SDL_RenderDrawPoint(ren, x, y);
			}

		renderFPS(time, delta);
		{
			helpers::vec2<int> spp = {int(pp.x*8)+128, 8-int(pp.z*8)+128};
			RenderFillRect(oren, {spp.x, spp.y, 5, 5});
			renderText(vts(pp), 15, 60);

			RenderDrawCircle(oren, int(sph.pos.x*8)+128, 8-int(sph.pos.z*8)+128, sph.r*4); 
			renderText(vts(sph.pos), 15, 75);

			v2f b1 = v2f{std::cos(-FOV-M_PI_2f32), std::sin(-FOV-M_PI_2f32)}, b2 = v2f{std::cos(FOV-M_PI_2f32), std::sin(FOV-M_PI_2f32)};
			SDL_RenderDrawLine(oren, spp.x, spp.y, int(spp.x+b1.x*50), int(spp.y+b1.y*50));
			SDL_RenderDrawLine(oren, spp.x, spp.y, int(spp.x+b2.x*50), int(spp.y+b2.y*50));
		}
		SDL_RenderPresent(oren);
	};

	loop();
	return 0;
}
