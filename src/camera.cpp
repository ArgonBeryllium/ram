#include "camera.h"
#include "globals.h"

v3f Camera::pos = {0,0,-5};
v3f Camera::angles = {};
float Camera::fov = M_PIf32/4;
float Camera::plane_offset = .2;
void Camera::init() {};

static v2f getPlaneDims()
{
	return v2f{std::tan(Camera::fov/2 - M_PI_2f32), -float(H)/W*std::tan(Camera::fov/2 - M_PI_2f32)}*Camera::plane_offset*-2;
}
v3f Camera::getPlaneCoord(const v2i &pp)
{
	v2f pd = getPlaneDims();
	float dx = float(pp.x*2 - W)/W*pd.x, dy = float(pp.y*2-H)/H*pd.y;
	return pos+v3f{dx, dy, plane_offset};
}
v3f Camera::getRayDir(const v2i &pp)
{
	float xs = float(pp.x*2 - W)/W;
	float ys = float(pp.y*2 - H)/H;
	float xa = fov/2*xs - M_PI_2f32 + angles.y, ya = fov/2*ys - M_PI_2f32 + angles.x;
	return v3f{std::cos(xa), -std::cos(ya),std::min(1.f, std::sin(ya)*std::sin(xa))};
}
