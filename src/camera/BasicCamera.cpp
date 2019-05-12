//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"

Ray BasicCamera::shootRayAt(double i, double j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;
	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	return {pos, Dir(ex * xs + ey * ys + ez * length)};
}

