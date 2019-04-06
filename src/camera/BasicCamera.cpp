//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"

BasicCamera::BasicCamera(const Pos &pos_, const ElAg &euler_angles_,
						 unsigned int width_, unsigned int height_, double fovx_) :
		Camera(pos_, euler_angles_, width_, height_, fovx_)
{
}

Ray BasicCamera::shootRayAt(unsigned int i, unsigned int j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
//	double xs = (i - w_2) * PIXEL_SIZE, ys = (j - h_2) * PIXEL_SIZE;
	double xs = (2.0 * i - width) / width * tan(fovx), ys = (2.0 * j - height) / height + tan(fovy);

	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	Dir &&dir = ex * xs + ey * ys + ez;
	dir.unitize();    // todo cancel

	// calculate ray.org
	// solve t ez.dir = F ez.ez
//	double t = CAMERA_FOCUS / (ez % dir);
//	Pos &&org = this->pos;

	return {pos, dir};
}
