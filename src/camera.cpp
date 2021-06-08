#include "camera.h"
#include "globals.h"

v3f Camera::pos = {0,0,-5};
v3f Camera::angles = {};
float Camera::fov = M_PI_2f32;
float Camera::plane_offset = 1;
void Camera::init()
{
};

static v2f getPlaneDims()
{
	return v2f{std::tan(Camera::fov/2 - M_PI_2f32), -float(H)/W*std::tan(Camera::fov/2 - M_PI_2f32)}*Camera::plane_offset*-2;
}
v3f Camera::projectPtoS(const v2i &pp, float zd)
{
	v2f pd = getPlaneDims();
	return pos+v3f{float(pp.x*2 - W)/W*pd.x, float(pp.y*2-H)/H*pd.y, plane_offset+zd};
}
v3f Camera::getRayDir(const v2i &pp)
{
	float xs = float(pp.x*2 - W)/W;
	float ys = float(pp.y*2 - H)/H;
	return v3f{std::cos(fov/2*xs - M_PI_2f32), -std::cos(fov/2*ys - M_PI_2f32),std::min(1.f, std::sin(fov/2*ys - M_PI_2f32)*std::sin(fov/2*xs - M_PI_2f32))};
};
