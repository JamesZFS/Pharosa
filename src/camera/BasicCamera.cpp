//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"
#include "../utils/funcs.hpp"

Ray BasicCamera::shootRayAt(double i, double j, double sigma) const
{
	using Funcs::randfNormal;
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2 + randfNormal(0, sigma)) * pixel_size, ys = (j - h_2 + randfNormal(0, sigma)) * pixel_size;
	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	Dir &&dir = ex * xs + ey * ys + ez * focus;
	dir.unitize();    // todo
	return {pos, dir};
}

Ray BasicCamera::shootRayAt(double i, double j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;
	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	Dir &&dir = ex * xs + ey * ys + ez * focus;
	dir.unitize();    // todo
	return {pos, dir};
}

