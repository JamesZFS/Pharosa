//
// Created by James on 2019/4/2.
//

#include "DOFCamera.h"
#include "../utils/funcs.hpp"

using Funcs::randfNormal;

Ray DOFCamera::shootRayAt(double i, double j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;
	double xp = xs + randfNormal(0, aperture) * pixel_size, yp = ys + randfNormal(0, aperture) * pixel_size;
	double xq = xs * (length + focus) / length, yq = ys * (length + focus) / length;
	// randomly shoot ray
	return {	// todo simplify
			pos + ex * xp + ey * yp + ez * length,		// org
			ex * (xq - xp) + ey * (yq - yp) + ez * focus	// dir
	};
}
