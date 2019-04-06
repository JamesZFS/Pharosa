//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"

double BasicCamera::PIXEL_SIZE = 0.1, BasicCamera::FOCUS = 140.0;

BasicCamera::BasicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_) :
		Camera(pos_, euler_angles_, width_, height_)
{
}

Ray BasicCamera::shootRayAt(double i, double j, double sigma) const
{
	using Funcs::randfNormal;
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2 + randfNormal(0, sigma)) * PIXEL_SIZE, ys = (j - h_2 + randfNormal(0, sigma)) * PIXEL_SIZE;

	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	Dir &&dir = ex * xs + ey * ys + ez * FOCUS;
	dir.unitize();    // todo

	return {pos, dir};
}
