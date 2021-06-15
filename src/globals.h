#pragma once
#include "operators.h"
#include <vector>

constexpr inline int RW = 540, RH = 480, PS=5,
						W = RW/PS, H = RH/PS;
inline v3f lp = {3, 1, -4};
inline float li = 20;
struct SDF;
inline std::vector<SDF*> world;
inline const FloatCol GI = {.2,.2,.2,1};
