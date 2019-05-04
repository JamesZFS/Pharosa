//
// Created by James on 2019/4/5.
//

#include "OrthoCamera.h"
#include "../utils/funcs.hpp"

Ray OrthoCamera::shootRayAt(double i, double j, double sigma) const
{
	using Funcs::randfNormal;
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	// shoot from infinitively far perspective
	double xs = (i - w_2 + randfNormal(0, sigma)) * pixel_size, ys = (j - h_2 + randfNormal(0, sigma)) * pixel_size;

	return {pos + ex * xs + ey * ys, ez};
}

Ray OrthoCamera::shootRayAt(double i, double j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	// shoot from infinitively far perspective
	double xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;

	return {pos + ex * xs + ey * ys, ez};
}
