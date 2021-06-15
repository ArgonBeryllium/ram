#pragma once
#pragma clang diagnostic ignored "-Wc++17-extensions"
#include "operators.h"
#include <vector>

constexpr inline int RW = 540, RH = 480, PS=5,
						W = RW/PS, H = RH/PS;

struct SDF;
struct Light;
inline std::vector<SDF*> world;
inline std::vector<Light*> lights;
inline FloatCol GI = {.2,.2,.2,1};
