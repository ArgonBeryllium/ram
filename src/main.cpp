#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <shitrndr.h>
#include <vector>
#include <limits.h>

#include "rat.h"
using namespace shitrndr;

constexpr int RW = 540, RH = 480, PS = 5, W = RW/PS, H = RH/PS;
World world;

v3f iv;
int main()
{
	// shitrndr setup //
	{
		init(".", RW, RH, 1, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		WindowProps::setPixScale(PS);
		WindowProps::setLockType(WindowProps::BARS);
		WindowProps::setLocked(1);
		silentDefs();
		bg_col = {0,0,0,0};
	}
	
	#ifdef RAT_DEBUG
	initHelpers();
	#endif
	
	// scene setup //
	{
		world.cam->output_dims.x = W;
		world.cam->output_dims.y = H;
		
		Shader_Def* red_n_shiny = new Shader_Def();
		red_n_shiny->col = {1,0,0,1};
		red_n_shiny->smooth = .8;
		red_n_shiny->reflective = .9;
		
		world.solids.push_back(new Sphere(&world));
		world.solids[0]->shader = red_n_shiny;
		
		world.solids.push_back(new Sphere(&world, {0,-5,0}, 3));
		world.solids.push_back(new Sphere(&world, {0,-3,4}, 3));
		world.solids.push_back(new Plane(&world, {0,-3,0}));

		world.solids.push_back(new Sphere(&world, {-6,0,-5}, 3));
		
		world.lights.push_back(new Light{{}, 10});
		world.lights.push_back(new Light{{-20, 20, -4}, 400.f, {1, .9, .8}});
	}
	
	// main loop
	onRender = [](double delta, double time)
	{
		// debug window stuff
		{
			#ifdef RAT_DEBUG
			SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
			SDL_RenderClear(oren);
			SetRenderColour(oren, {255,255,255,255});
			#endif
		}
		
		// animations //
		{
			world.lights[0]->col.r = std::abs(std::sin(time))  *10;
			world.lights[0]->col.g = std::abs(std::cos(time))  *10;
			world.lights[0]->col.b = std::abs(std::sin(time+1))*10;
			
			world.lights[0]->pos.x = std::cos(time)*5;
			world.lights[0]->pos.y = std::cos(time)*2 + 1;
			world.lights[0]->pos.z = std::sin(time)*5;
			
			world.solids[0]->pos.y = std::sin(time+.2);
		}
		
		// camera controls //
		{
			iv = {((gk(SDLK_d) || gk(SDLK_RIGHT))?1.f:0.f) - ((gk(SDLK_a) || gk(SDLK_LEFT))?1.f:0.f),
					((gk(SDLK_w) || gk(SDLK_UP))?1.f:0.f) - ((gk(SDLK_s) || gk(SDLK_DOWN))?1.f:0.f)};
			world.cam->pos += (gk(SDLK_LSHIFT)?world.cam->up():world.cam->forward())*iv.y*3*delta;
			world.cam->pos += world.cam->right()*iv.x*3*delta;
			
			if(Input::getKey(SDLK_j)) world.cam->angles.y += delta*2;
			if(Input::getKey(SDLK_l)) world.cam->angles.y -= delta*2;
			if(Input::getKey(SDLK_i)) world.cam->angles.x += delta*2;
			if(Input::getKey(SDLK_k)) world.cam->angles.x -= delta*2;

			//world.cam->plane_offset = float(Input::getMP().y)*5*5/H;
		}
		
		std::srand(time*10000); // otherwise every frame it'd remain set to the last pixel's seed
		world.cam->renderFrame();
		
		// more debug window stuff
		{
			#ifdef RAT_DEBUG
			renderFPS(time, delta);
			
			v2i cp = getHelperCoords(world.cam->pos);
			RenderFillCircle(oren, cp.x, cp.y, 2);
			SDL_SetRenderDrawColor(oren, 255, 0, 0, 255);
			v2i co = getHelperCoords(world.cam->pos-v3f{0,0,world.cam->plane_offset});
			RenderFillCircle(oren, co.x, co.y, 2);
			SDL_RenderDrawLine(oren, co.x, co.y, cp.x, cp.y);

			SDL_SetRenderDrawColor(oren, 255, 255, 255, 255);
			for(Solid* obj : world.solids)
				obj->renderPreview(oren);
			
			v2i slp = getHelperCoords(world.lights[0]->pos);
			RenderFillCircle(oren, slp.x, slp.y, 2);
			
			SDL_RenderPresent(oren);
			#endif
		}
	};
	onKeyDown = [](const SDL_Keycode& key)
	{
		if(key==SDLK_q) std::exit(0);
	};
	#ifdef RAT_DEBUG
	onEvent = [](SDL_Event* e)
	{
		switch(e->type)
		{
			case SDL_MOUSEWHEEL :
				if(e->wheel.y>0)
					oscale *= 1.5;
				else
					oscale /= 1.5;
				break;
		}
	};
	#endif
	
	loop();
	return 0;
}
