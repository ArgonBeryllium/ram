#pragma once
#include <SDL2/SDL.h>

inline SDL_Colour operator+(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r+b.r), Uint8(a.g+b.g), Uint8(a.b+b.b), Uint8(a.a+b.a)}; }
inline SDL_Colour operator-(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r-b.r), Uint8(a.g-b.g), Uint8(a.b-b.b), Uint8(a.a-b.a)}; }
inline SDL_Colour operator*(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r*b.r), Uint8(a.g*b.g), Uint8(a.b*b.b), Uint8(a.a*b.a)}; }
inline SDL_Colour operator/(const SDL_Colour& a, const SDL_Colour& b) { return SDL_Colour{Uint8(a.r/b.r), Uint8(a.g/b.g), Uint8(a.b/b.b), Uint8(a.a/b.a)}; }

inline SDL_Colour operator*(const SDL_Colour& a, const int& b) { return SDL_Colour{Uint8(a.r*b), Uint8(a.g*b), Uint8(a.b*b), Uint8(a.a*b)}; }
inline SDL_Colour operator/(const SDL_Colour& a, const int& b) { return SDL_Colour{Uint8(a.r/b), Uint8(a.g/b), Uint8(a.b/b), Uint8(a.a/b)}; }

inline void operator+=(SDL_Colour& a, const SDL_Colour& b) { a = a+b; }
inline void operator-=(SDL_Colour& a, const SDL_Colour& b) { a = a-b; }
inline void operator*=(SDL_Colour& a, const SDL_Colour& b) { a = a*b; }
inline void operator/=(SDL_Colour& a, const SDL_Colour& b) { a = a/b; }

inline void operator*=(SDL_Colour& a, const int& b) { a = a*b; }
inline void operator/=(SDL_Colour& a, const int& b) { a = a/b; }
