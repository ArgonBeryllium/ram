#include <shitrndr.h>
#include <vector>
#include "ram.h"
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
	
	#ifdef RAM_DEBUG
	initHelpers();
	#endif
	
	// scene setup //
	{
		world.cam->output_dims.x = W;
		world.cam->output_dims.y = H;
		
		world.sdfs.push_back(new Sphere(&world));
		world.sdfs.push_back(new Cuboid(&world, {0,-2.5,0}, {3,1,3}));
		world.sdfs.push_back(new Sphere(&world, {0,-3,4}, 3));
		world.sdfs.push_back(new Cuboid(&world, {0,-3.5,0}, {100,1,100}));
		world.sdfs.push_back(new Sphere(&world, {-8,0,3}, 3));

		Shader_Def* red_n_shiny = new Shader_Def();
		red_n_shiny->col = {1,0,0,1};
		red_n_shiny->smooth = .8;
		red_n_shiny->reflective = .9;
		world.sdfs[0]->shader = red_n_shiny;

		((Cuboid*)world.sdfs[1])->rounding = .2;

		struct S_Checkerboard : Shader_Def
		{
			FloatCol getPixelValue(SDF* obj, const Ray &ray, const Intersection &o_i, uint32_t rec) override
			{
				FloatCol out;
				out = Shader_Def::getPixelValue(obj, ray, o_i, rec);
					
				out *= std::max(.2f, float(std::abs(int(o_i.surf_point.x+10000)+int(o_i.surf_point.z+10000))%2));
				return out;
			};
		};
		world.sdfs[3]->shader = new S_Checkerboard();
		
		world.lights.push_back(new Light{{-2, 2, -3}, 8, {.3, .4, 1}});
		world.lights.push_back(new Light{{6, -1, -4}, 5});
		world.lights.push_back(new Light{{-4.5, 9, 7}, 30, {.9, .6, .1}});
	}
	
	// main loop
	onRender = [](double delta, double time)
	{
		// debug window stuff
		{
			#ifdef RAM_DEBUG
			SDL_SetRenderDrawColor(oren, 25, 5, 35, 255);
			SDL_RenderClear(oren);
			SetRenderColour(oren, {255,255,255,255});
			#endif
		}
		
		// animations //
		{
			world.sdfs[0]->pos.y = std::sin(time+.2);
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
			if(Input::getKey(SDLK_u)) world.cam->angles.z += delta*2;
			if(Input::getKey(SDLK_o)) world.cam->angles.z -= delta*2;

			//world.cam->plane_offset = float(Input::getMP().y)*5*5/H;
		}
		
		std::srand(time*10000); // otherwise every frame it'd remain set to the last pixel's seed
		world.cam->renderFrame();
		
		// more debug window stuff
		{
			#ifdef RAM_DEBUG
			renderFPS(time, delta);
			
			v2i cp = getHelperCoords(world.cam->pos);
			RenderFillCircle(oren, cp.x, cp.y, 2);
			SDL_SetRenderDrawColor(oren, 255, 0, 0, 255);
			v2i co = getHelperCoords(world.cam->pos-v3f{0,0,world.cam->plane_offset});
			RenderFillCircle(oren, co.x, co.y, 2);
			SDL_RenderDrawLine(oren, co.x, co.y, cp.x, cp.y);
			
			SDL_SetRenderDrawColor(oren, 255, 255, 255, 255);
			for(SDF* sdf : world.sdfs)
				sdf->renderPreview(oren);
			
			for(Light* l : world.lights)
			{
				v2i slp = getHelperCoords(l->pos);
				RenderFillCircle(oren, slp.x, slp.y, 2);
			}
			
			SDL_RenderPresent(oren);
			#endif
		}
	};
	onKeyDown = [](const SDL_Keycode& key)
	{
		if(key==SDLK_q) std::exit(0);
	};
	#ifdef RAM_DEBUG
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
