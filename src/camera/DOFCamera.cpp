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
	double xd = randfNormal(0, aperture), yd = randfNormal(0, aperture);
//	double xp = xs + xd, yp = ys + yd;
//	double xq = xs * (length + focus) / length, yq = ys * (length + focus) / length;
	// randomly shoot L1
	return {    // todo simplify
			pos_ez_length + ex * (xs + xd) + ey * (ys + yd),        // org
			ez_focus + ex * (xs * f_l - xd) + ey * (ys * f_l - yd)    // dir
	};
}
