#pragma once
#include <SDL2/SDL.h>

// A colour struct capable of values beyond 255.
// Meant to be used for accumulation, e.g. when using more than 1 sample per ray
struct MegaCol
{
	uint32_t r = 0, g = 0, b = 0, a = 255;
	SDL_Colour col() { return {Uint8(r), Uint8(g), Uint8(b), Uint8(a) }; }
};

inline MegaCol operator+(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a}; }
inline MegaCol operator-(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a}; }
inline MegaCol operator*(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r*b.r, a.g*b.g, a.b*b.b, a.a*b.a}; }
inline MegaCol operator/(const MegaCol& a, const SDL_Colour& b) { return MegaCol{a.r/b.r, a.g/b.g, a.b/b.b, a.a/b.a}; }

inline MegaCol operator*(const MegaCol& a, const int& b) { return MegaCol{a.r*b, a.g*b, a.b*b, a.a*b}; }
inline MegaCol operator/(const MegaCol& a, const int& b) { return MegaCol{a.r/b, a.g/b, a.b/b, a.a/b}; }

inline void operator+=(MegaCol& a, const SDL_Colour& b) { a = a+b; }
inline void operator-=(MegaCol& a, const SDL_Colour& b) { a = a-b; }
inline void operator*=(MegaCol& a, const SDL_Colour& b) { a = a*b; }
inline void operator/=(MegaCol& a, const SDL_Colour& b) { a = a/b; }

inline void operator*=(MegaCol& a, const int& b) { a = a*b; }
inline void operator/=(MegaCol& a, const int& b) { a = a/b; }

struct FloatCol
{
	float r = 1, g = 1, b = 1, a = 1;
	SDL_Colour col() const { return {Uint8(r*255), Uint8(g*255), Uint8(b*255), Uint8(a*255) }; }
};

inline FloatCol operator+(const FloatCol& a, const SDL_Colour& b) { return FloatCol{a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a}; }
inline FloatCol operator-(const FloatCol& a, const SDL_Colour& b) { return FloatCol{a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a}; }
inline FloatCol operator*(const FloatCol& a, const SDL_Colour& b) { return FloatCol{a.r*b.r, a.g*b.g, a.b*b.b, a.a*b.a}; }
inline FloatCol operator/(const FloatCol& a, const SDL_Colour& b) { return FloatCol{a.r/b.r, a.g/b.g, a.b/b.b, a.a/b.a}; }

inline FloatCol operator*(const FloatCol& a, const float& b) { return FloatCol{a.r*b, a.g*b, a.b*b, a.a*b}; }
inline FloatCol operator/(const FloatCol& a, const float& b) { return FloatCol{a.r/b, a.g/b, a.b/b, a.a/b}; }

inline void operator+=(FloatCol& a, const SDL_Colour& b) { a = a+b; }
inline void operator-=(FloatCol& a, const SDL_Colour& b) { a = a-b; }
inline void operator*=(FloatCol& a, const SDL_Colour& b) { a = a*b; }
inline void operator/=(FloatCol& a, const SDL_Colour& b) { a = a/b; }

inline void operator*=(FloatCol& a, const float& b) { a = a*b; }
inline void operator/=(FloatCol& a, const float& b) { a = a/b; }

