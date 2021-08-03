#include <cmath>
#include "camera.h"
#include "ray.h"
#include "world.h"

void Camera::renderFrame(SDL_Rect* rect, size_t samples, SDL_Renderer* target)
{
	int rw, rh, xo, yo;
	if(rect)
	{
		rw = rect->w;
		rh = rect->h;
		xo = rect->x;
		yo = rect->y;
	}
	else
	{
		rw = output_dims.x;
		rh = output_dims.y;
		xo = yo = 0;
	}

	for (int y = 0; y != rh; y++)
		for (int x = 0; x != rw; x++)
		{
			MegaCol col;
			for (size_t i = 0; i != samples; i++)
				col += castRay(x/float(rw)*output_dims.x, y/float(rh)*output_dims.y, x+y*output_dims.x+std::rand()+i);
			col /= samples;
			SDL_Colour pix = col.col();
			
			if(pix.a==0) continue;
			SDL_SetRenderDrawColor(target, pix.r, pix.g, pix.b, pix.a);
			SDL_RenderDrawPoint(target, x+xo, y+yo);
		}
}

inline v2f Camera::getPlaneDims()
{
	return v2f{1, -float(output_dims.x)/output_dims.y}*std::atan(fov/2)*2*plane_offset;
}
v3f Camera::getPlaneCoord(const v2i &pp)
{
	v2f pd = getPlaneDims();
	float dx = float(pp.x*2 - output_dims.x)/output_dims.x*pd.x, dy = float(pp.y*2-output_dims.y)/output_dims.y*pd.y;
	return pos+rotatePoint(v3f{dx, dy, 0}, angles);
}
v3f Camera::getRayDir(const v2i &pp)
{
	float xs = float(pp.x*2 - output_dims.x)/output_dims.x * float(output_dims.x)/output_dims.y;
	float ys = float(pp.y*2 - output_dims.y)/output_dims.y;
	float tfot = std::tan(fov/2);
	return rotatePoint(v3f{xs*tfot, -ys*tfot, 1}, angles).normalised();
}

SDL_Colour Camera::castRay(int x, int y, int seed)
{
	using namespace shitrndr;
	std::srand(seed);
	SDL_Colour out = parent_world->global_illum.col();
	
	Ray ray = {getPlaneCoord({x,y}), getRayDir({x,y})};
	
	#ifdef RAT_DEBUG
	v2i ro = getHelperCoords(ray.ori);
	v2i re = ro+(getHelperDir(ray.dir)*512).to<int>();
	SetRenderColour(oren, {255,255,255,1});
	#endif
	
	float sd = MAXFLOAT;
	for(Solid* obj : parent_world->solids)
	{
		Intersection i = obj->getIntersection(ray);
		
		if(i.intersecting && i.min_dist>0 && i.min_dist<sd)
		{
			sd = i.min_dist;
			out = obj->shader->getPixelValue(obj, ray, i);
			
			#ifdef RAT_DEBUG
			SetRenderColour(oren, {255,0,0,1});
			#endif
		}
	}
	
	#ifdef RAT_DEBUG
	SDL_RenderDrawLine(oren, ro.x, ro.y, re.x, re.y);
	SetRenderColour(oren, {255,255,255,255});
	#endif
	
	return out;
}
