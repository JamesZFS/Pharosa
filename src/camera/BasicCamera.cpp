//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"

Ray BasicCamera::shootRayAt(real i, real j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	real xs = (i - w_2) * pixel_size, ys = (j - h_2) * pixel_size;
	// from screen center crd (x', y') get global crd of L1.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
//	return {pos_ez_length + tmp, ez_length + ex * xs + ey * ys};
	Pos tmp = ex * xs + ey * ys;
#if CAMERA_PUSH_IN
	Ray ray(pos_ez_length + tmp, ez_length + tmp);
#else
	Ray ray(pos, ez_length + tmp);
#endif
	ray.dir.unitize();
	return ray;
}
