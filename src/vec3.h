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
