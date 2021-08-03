#pragma once
#define RAT_DEBUG

#include <shitrndr.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include "operators.h"

#define gk(X) shitrndr::Input::getKey(X)
using v2f = shitrndr::helpers::vec2<float>;
using v2i = shitrndr::helpers::vec2<int>;

inline float frand() { return float(std::rand())/float(RAND_MAX); }
inline float frange() { return (frand()*2)-1; }

#ifdef RAT_DEBUG
#include "vec3.h"
inline SDL_Window* owin;
inline SDL_Renderer* oren;
inline float oscale=6;

inline TTF_Font* font;
inline void renderText(const std::string& text, int x, int y, bool cache = 0, SDL_Colour col = {255,255,255,255}, SDL_Renderer* r = oren)
{
	using namespace shitrndr;
	static std::map<std::string, SDL_Texture*> cached;
	SDL_Texture* t;
	if(cache && cached[text])
		t = cached[text];
	else
	{
		SDL_Surface* s = TTF_RenderText_Solid(font, text.c_str(), col);
		t = SDL_CreateTextureFromSurface(r, s);
		SDL_FreeSurface(s);
		if(cache) cached[text] = t;
	}
	int w, h;
	SDL_QueryTexture(t,0,0,&w,&h);
	RenderCopy(r, t, {x,y,w,h});
	if(!cache) SDL_DestroyTexture(t);
}

inline void initHelpers()
{
	TTF_Init();
	font = TTF_OpenFont("res/ProggyTiny.ttf", 30);

	SDL_CreateWindowAndRenderer(256, 256, 0, &owin, &oren);
	int w, h;
	SDL_GetWindowPosition(shitrndr::win, &w, &h);
	SDL_SetWindowPosition(owin, 960-w/2-256,540-h/2);
	SDL_SetRenderDrawBlendMode(oren, SDL_BLENDMODE_BLEND);

}
inline void renderFPS(float time, float delta)
{
	static std::string ctt;
	static double last = 0;
	static int fc = 0;
	fc++;
	if(time-last>=1)
	{
		ctt = std::to_string(fc);
		last = time;
		fc = 0;
	}
	renderText(ctt, 0, 0);
	renderText("("+std::to_string(delta)+")", 0, 30);
	renderText("___________", 0, 40);
}
inline v2f getHelperDir(v3f d)
{
	if(shitrndr::Input::getKey(SDLK_LSHIFT)) return v2f(d.z, -d.y)*oscale;
	if(shitrndr::Input::getKey(SDLK_LCTRL)) return v2f(d.x, -d.y)*oscale;
	return v2f(d.x, -d.z)*oscale;
}
inline v2i getHelperCoords(v3f p)
{
	return getHelperDir(p).to<int>()+v2i{128,128};
}
#endif
