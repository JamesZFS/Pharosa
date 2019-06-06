//
// Created by James on 2019/4/5.
//

#include "OrthoCamera.h"
#include "../utils/funcs.hpp"

Ray OrthoCamera::shootRayAt(real i, real j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	// shoot from infinitively far perspective
	real xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;

	return {pos + ex * xs + ey * ys, ez};
}
