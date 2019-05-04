//
// Created by James on 2019/4/2.
//

#include "DOFCamera.h"
#include "../utils/funcs.hpp"

using Funcs::randf;


Ray DOFCamera::shootRayAt(double i, double j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	double xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;
	// randomly shoot ray
	double r1 = randf(2 * M_PI), r2 = randf(), r2s = sqrt(r2), t = length / sqrt(1 - r2);
	// hit the lens
	Pos &&P = ex * (xs + cos(r1) * r2s * t) + ey * (ys + sin(r1) * r2s * t) + ez * length;
	Pos &&Q = ex * (beta * xs) + ey * (beta * ys) + ez * u_v;	// image
	return Ray::between(P, Q);
}
