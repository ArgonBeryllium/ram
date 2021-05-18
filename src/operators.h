#pragma once
#include <SDL2/SDL.h>
#include <string>

SDL_Colour operator+(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r+b.r), Uint8(a.g+b.g), Uint8(a.b+b.b), Uint8(a.a+b.a)}; }
SDL_Colour operator-(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r-b.r), Uint8(a.g-b.g), Uint8(a.b-b.b), Uint8(a.a-b.a)}; }
SDL_Colour operator*(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r*b.r), Uint8(a.g*b.g), Uint8(a.b*b.b), Uint8(a.a*b.a)}; }
SDL_Colour operator/(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r/b.r), Uint8(a.g/b.g), Uint8(a.b/b.b), Uint8(a.a/b.a)}; }

SDL_Colour operator*(const SDL_Colour& a, const int& b) { return SDL_Colour{Uint8(a.r*b), Uint8(a.g*b), Uint8(a.b*b), Uint8(a.a*b)}; }
SDL_Colour operator/(const SDL_Colour& a, const int& b) { return SDL_Colour{Uint8(a.r/b), Uint8(a.g/b), Uint8(a.b/b), Uint8(a.a/b)}; }

void operator+=(SDL_Colour& a, const SDL_Colour& b) { a = a+b; }
void operator-=(SDL_Colour& a, const SDL_Colour& b) { a = a-b; }
void operator*=(SDL_Colour& a, const SDL_Colour& b) { a = a*b; }
void operator/=(SDL_Colour& a, const SDL_Colour& b) { a = a/b; }

void operator*=(SDL_Colour& a, const int& b) { a = a*b; }
void operator/=(SDL_Colour& a, const int& b) { a = a/b; }

struct MegaCol
{
	uint32_t r = 0, g = 0, b = 0, a = 255;
	SDL_Colour col() { return {Uint8(r), Uint8(g), Uint8(b), Uint8(a) }; }
};

MegaCol operator+(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a}; }
MegaCol operator-(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a}; }
MegaCol operator*(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r*b.r, a.g*b.g, a.b*b.b, a.a*b.a}; }
MegaCol operator/(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r/b.r, a.g/b.g, a.b/b.b, a.a/b.a}; }
                        
MegaCol operator*(const MegaCol& a, const int& b) { return MegaCol{a.r*b, a.g*b, a.b*b, a.a*b}; }
MegaCol operator/(const MegaCol& a, const int& b) { return MegaCol{a.r/b, a.g/b, a.b/b, a.a/b}; }

void operator+=(MegaCol& a, const SDL_Colour& b) { a = a+b; }
void operator-=(MegaCol& a, const SDL_Colour& b) { a = a-b; }
void operator*=(MegaCol& a, const SDL_Colour& b) { a = a*b; }
void operator/=(MegaCol& a, const SDL_Colour& b) { a = a/b; }

void operator*=(MegaCol& a, const int& b) { a = a*b; }
void operator/=(MegaCol& a, const int& b) { a = a/b; }


struct v3f
{
	float x, y, z;
	float getLengthSquared() { return x*x+y*y+z*z; }
	float getLength() { return std::sqrt(getLengthSquared()); }
	v3f normalised();
};
v3f operator+(const v3f& a, const v3f& b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
v3f operator-(const v3f& a, const v3f& b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
v3f operator*(const v3f& a, const v3f& b) { return {a.x*b.x, a.y*b.y, a.z*b.z}; }
v3f operator/(const v3f& a, const v3f& b) { return {a.x/b.x, a.y/b.y, a.z/b.z}; }

v3f operator+(const v3f& a, const float& b) { return {a.x+b, a.y+b, a.z+b}; }
v3f operator-(const v3f& a, const float& b) { return {a.x-b, a.y-b, a.z-b}; }
v3f operator*(const v3f& a, const float& b) { return {a.x*b, a.y*b, a.z*b}; }
v3f operator/(const v3f& a, const float& b) { return {a.x/b, a.y/b, a.z/b}; }

void operator+=(v3f& a, const v3f& b) { a = a+b; }
void operator-=(v3f& a, const v3f& b) { a = a-b; }
void operator*=(v3f& a, const v3f& b) { a = a*b; }
void operator/=(v3f& a, const v3f& b) { a = a/b; }

void operator*=(v3f& a, const float& b) { a = a*b; }
void operator/=(v3f& a, const float& b) { a = a/b; }

float dot(const v3f& a, const v3f& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
std::string vts(const v3f& v) { return "{"+std::to_string(v.x)+", "+std::to_string(v.y)+", "+std::to_string(v.z)+"}";}

v3f v3f::normalised() { return (*this)/getLength(); }
