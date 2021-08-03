#pragma once
#include "operators.h"
#include "vec3.h"
#include "colours.h"
#include "helpers.h"

struct Light
{
	v3f pos;
	float intensity = 20;
	FloatCol col = {1,1,1,1};
};

struct SDF;
struct Ray;
struct Intersection;
struct Shader
{
	static Shader* def_inst; // default instance
	virtual FloatCol getPixelValue(SDF* obj, const Ray &ray, const Intersection &i, uint32_t rec = 0) = 0;
};
struct Shader_Def : Shader
{
	FloatCol col = {1,1,1,1};
	float smooth = .2;
	float reflective = .1;
	float ambient_occlusion = .01;
	
	FloatCol getPixelValue(SDF* obj, const Ray &ray, const Intersection &i, uint32_t rec = 0) override;
};

struct World;
struct SDF
{
	v3f pos;
	Shader* shader = Shader::def_inst;
	World* parent_world;
	SDF(World* parent_world_, v3f pos_) : parent_world(parent_world_), pos(pos_) {}

	virtual float getSurfaceDist(v3f p) = 0;
	virtual v3f getNormal(v3f p)
	{
		constexpr static float EPSILON = .0001;
		return v3f{
		getSurfaceDist(v3f{p.x + EPSILON, p.y, p.z}) - getSurfaceDist(v3f{p.x - EPSILON, p.y, p.z}),
		getSurfaceDist(v3f{p.x, p.y + EPSILON, p.z}) - getSurfaceDist(v3f{p.x, p.y - EPSILON, p.z}),
		getSurfaceDist(v3f{p.x, p.y, p.z  + EPSILON}) - getSurfaceDist(v3f{p.x, p.y, p.z - EPSILON})}.normalised();
	}

	virtual ~SDF() {}

	#ifdef RAM_DEBUG
	virtual void renderPreview(SDL_Renderer* r) = 0;
	#endif
};

struct Sphere : SDF
{
	float radius = 1;
	Sphere(World* parent_world_, v3f pos_= {}, float radius_ = 1) : SDF(parent_world_, pos_), radius(radius_) {}

	float getSurfaceDist(v3f p) override { return (p-pos).getLength()-radius; }
	v3f getNormal(v3f surface_point) override { return (surface_point - pos).normalised(); }

	#ifdef RAM_DEBUG
	void renderPreview(SDL_Renderer* r) override
	{
		v2i sp = getHelperCoords(pos);
		shitrndr::RenderDrawCircle(r, sp.x, sp.y, radius*oscale);
	}
	#endif
};
struct Cuboid : SDF
{
	v3f dims;
	float rounding = 0;
	Cuboid(World* parent_world_, v3f pos_ = {}, v3f dims_ = {1,1,1}) : SDF(parent_world_, pos_), dims(dims_) {}

	float getSurfaceDist(v3f p) override
	{
		p -= pos;
		p.x = std::abs(p.x);
		p.y = std::abs(p.y);
		p.z = std::abs(p.z);
		v3f d = p-dims/2;
		d.x = std::max(d.x, 0.f);
		d.y = std::max(d.y, 0.f);
		d.z = std::max(d.z, 0.f);
		return d.getLength() - rounding;
	}

	#ifdef RAM_DEBUG
	void renderPreview(SDL_Renderer* r) override
	{
		v2i pp = getHelperCoords(pos);
		v2i pd = getHelperDir(dims).to<int>();
		pp -= pd/2;
		shitrndr::RenderDrawRect(r, {pp.x, pp.y, pd.x, pd.y});
	}
	#endif
};
