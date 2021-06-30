#include "camera.h"
#include "ray.h"
#include "world.h"

v2f Camera::getPlaneDims()
{
	return v2f{std::tan(Camera::fov/2 - M_PI_2f32), -float(output_dims.y)/output_dims.x*std::tan(Camera::fov/2 - M_PI_2f32)}*Camera::plane_offset*-2;
}
v3f Camera::getPlaneCoord(const v2i &pp)
{
	v2f pd = getPlaneDims();
	float dx = float(pp.x*2 - output_dims.x)/output_dims.x*pd.x, dy = float(pp.y*2-output_dims.y)/output_dims.y*pd.y;
	return pos+v3f{dx, dy, plane_offset};
}
v3f Camera::getRayDir(const v2i &pp)
{
	float xs = float(pp.x*2 -output_dims.x)/output_dims.x;
	float ys = float(pp.y*2 - output_dims.y)/output_dims.y;
	float xa = fov/2*xs - M_PI_2f32 + angles.y, ya = fov/2*ys - M_PI_2f32 + angles.x;
	return v3f{std::cos(xa), -std::cos(ya),std::min(1.f, std::sin(ya)*std::sin(xa))};
}

SDL_Colour Camera::castRay(int x, int y, int seed)
{
	using namespace shitrndr;
	std::srand(seed);
	SDL_Colour out = parent_world->global_illum.col();

	Ray ray = {getPlaneCoord({x,y}), getRayDir({x,y})};

#ifdef DEBUG_WINDOW
	v2i ro = getoutput_dims.yelperCoords(ray.ori);
	v2i re = ro+(getoutput_dims.yelperDir(ray.dir)*512).to<int>();
	SetRenderColour(oren, {255,255,255,1});
#endif

	float sd = MAXFLOAT;
	for(Solid* obj : parent_world->solids)
	{
		Intersection i = obj->getIntersection(ray);

#ifdef DEBUG_WINDOW
		SetRenderColour(oren, {255,255,255,1});
#endif
		if(i.intersecting && i.min_dist>0 && i.min_dist<sd)
		{
			sd = i.min_dist;
			out = obj->shader->getPixelValue(obj, ray, i);

#ifdef DEBUG_WINDOW
			SetRenderColour(oren, {255,0,0,1});
#endif
		}

#ifdef DEBUG_WINDOW
		SDL_RenderDrawLine(oren, ro.x, ro.y, re.x, re.y);
#endif
	}
#ifdef DEBUG_WINDOW
	SetRenderColour(oren, {255,255,255,255});
#endif

	return out;
}
