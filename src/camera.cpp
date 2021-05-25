#include "camera.h"
#include "globals.h"

v3f Camera::pos = {0,0,-5};
float Camera::fov = M_PI_2f32;
float Camera::plane_offset = .5;
void Camera::init()
{
};

static v2f getPlaneDims()
{
	return {1, float(H)/W*1};
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
	return {std::cos(fov/2*xs - M_PI_2f32),std::cos(fov/2*ys - M_PI_2f32),1};
};