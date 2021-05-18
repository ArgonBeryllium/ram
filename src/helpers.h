#pragma once
#include <shitrndr.h>
#include <SDL2/SDL_ttf.h>
#include <map>

#define gk(X) shitrndr::Input::getKey(X)
using v2f = shitrndr::helpers::vec2<float>;

inline float frand() { return float(std::rand())/RAND_MAX; }
inline float frange() { return (frand()*2)-1; }

SDL_Window* owin;
SDL_Renderer* oren;

TTF_Font* font;
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

void initHelpers()
{
	TTF_Init();
	font = TTF_OpenFont("res/ProggyTiny.ttf", 25);

	SDL_CreateWindowAndRenderer(256, 256, 0, &owin, &oren);
}
void renderFPS(float time, float delta)
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
