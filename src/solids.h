#pragma once
#include "operators.h"
#include "vec3.h"
#include "colours.h"
#include "ray.h"
#include "helpers.h"

struct Light
{
	v3f pos;
	float intensity = 20;
	FloatCol col = {1,1,1,1};
};
struct Shader
{
	static Shader* def_inst; // default instance
	virtual SDL_Colour getPixelValue(Solid* obj, const Ray &ray, const Intersection &i, uint32_t rec = 0) = 0;
};
struct Shader_Def : Shader
{
	FloatCol col = {1,1,1,1};
	float smooth = .3;
	float reflective = .2;
	
	SDL_Colour getPixelValue(Solid* obj, const Ray &ray, const Intersection &i, uint32_t rec = 0) override;
};

struct World;
struct Solid
{
	v3f pos;
	Shader* shader = Shader::def_inst;
	World* parent_world;
	Solid(World* parent_world_, v3f pos_) : parent_world(parent_world_), pos(pos_) {}

	virtual Intersection getIntersection(Ray ray) = 0;
	virtual v3f getNormal(v3f surface_point) = 0;

	virtual ~Solid() {}

	#ifdef RAT_DEBUG
	virtual void renderPreview(SDL_Renderer* r) = 0;
	#endif
};

struct Sphere : Solid
{
	float radius = 1;
	Sphere(World* parent_world_, v3f pos_= {}, float radius_ = 1) : Solid(parent_world_, pos_), radius(radius_) {}
	Intersection getIntersection(Ray ray) override
	{
		v3f od = ray.ori-pos;
		float b = dot(ray.dir, od);
		float fac = dot(od, od) - radius*radius;
		float d = b*b - fac;
		if(d<0) return {0};
		
		float d1, d2;
		d1 = -b + std::sqrt(d);
		d2 = -b - std::sqrt(d);
		float min_dist = std::min(d1, d2);
		float max_dist = std::max(d1, d2);
		return Intersection{min_dist>0, min_dist, max_dist, (ray.ori+ray.dir*min_dist-pos).normalised(), this};
	};
	v3f getNormal(v3f surface_point) override { return (surface_point - pos).normalised(); }

	#ifdef RAT_DEBUG
	void renderPreview(SDL_Renderer* r) override
	{
		v2i sp = getHelperCoords(pos); 
		shitrndr::RenderDrawCircle(r, sp.x, sp.y, radius*oscale);
	}
	#endif
};

struct Plane : Solid
{
	v3f normal;
	Plane(World* parent_world_, v3f pos_ = {}, v3f normal_ = {0,1,0}) : Solid(parent_world_, pos_), normal(normal_) {}
	Intersection getIntersection(Ray ray) override
	{
		float dist = dot(pos-ray.ori, normal)/dot(ray.dir, normal);
		return Intersection{dist>0, dist, dist, normal, this};
	}
	v3f getNormal(v3f surface_point) override { return normal; }
	#ifdef RAT_DEBUG
	void renderPreview(SDL_Renderer* r) override
	{
		v2i sp = getHelperCoords(pos); 
		shitrndr::RenderFillCircle(oren, sp.x, sp.y, 4);
	}
	#endif
};
