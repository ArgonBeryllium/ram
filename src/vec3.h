#pragma once
#include <math.h>
#include <string>

struct v3f
{
	float x, y, z;
	float getLengthSquared() const { return x*x+y*y+z*z; }
	float getLength() const { return std::sqrt(getLengthSquared()); }
	v3f normalised();
};
inline v3f operator+(const v3f& a, const v3f& b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
inline v3f operator-(const v3f& a, const v3f& b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
inline v3f operator*(const v3f& a, const v3f& b) { return {a.x*b.x, a.y*b.y, a.z*b.z}; }
inline v3f operator/(const v3f& a, const v3f& b) { return {a.x/b.x, a.y/b.y, a.z/b.z}; }

inline v3f operator+(const v3f& a, const float& b) { return {a.x+b, a.y+b, a.z+b}; }
inline v3f operator-(const v3f& a, const float& b) { return {a.x-b, a.y-b, a.z-b}; }
inline v3f operator*(const v3f& a, const float& b) { return {a.x*b, a.y*b, a.z*b}; }
inline v3f operator/(const v3f& a, const float& b) { return {a.x/b, a.y/b, a.z/b}; }

inline void operator+=(v3f& a, const v3f& b) { a = a+b; }
inline void operator-=(v3f& a, const v3f& b) { a = a-b; }
inline void operator*=(v3f& a, const v3f& b) { a = a*b; }
inline void operator/=(v3f& a, const v3f& b) { a = a/b; }

inline void operator*=(v3f& a, const float& b) { a = a*b; }
inline void operator/=(v3f& a, const float& b) { a = a/b; }

inline float dot(const v3f& a, const v3f& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline std::string vts(const v3f& v) { return "{"+std::to_string(v.x)+", "+std::to_string(v.y)+", "+std::to_string(v.z)+"}";}

inline v3f v3f::normalised() { return (*this)/getLength(); }

inline v3f rotatePoint(v3f v, v3f angles, v3f origin = {})
{
	v3f out;
	v -= origin;
	float xm = std::sqrt(v.y*v.y + v.z*v.z);
	float ym = std::sqrt(v.z*v.z + v.x*v.x);
	float zm = std::sqrt(v.y*v.y + v.x*v.x);

	float xa = std::atan2(v.y, v.z) + angles.x;
	float ya = std::atan2(v.z, v.x) + angles.y;
	float za = std::atan2(v.y, v.x) + angles.z;

	out.x = std::cos(ya)*ym + std::cos(za)*zm;
	out.y = std::sin(xa)*xm + std::sin(za)*zm;
	out.z = std::cos(xa)*xm + std::sin(ya)*ym;
	return out*2 - v*2;
};
