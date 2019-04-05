//
// Created by James on 2019/4/5.
//

#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_,
									   unsigned int height_) : Camera(pos_, euler_angles_, width_, height_)
{}

Ray OrthographicCamera::shootRayAt(unsigned int i, unsigned int j) const
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	// shoot from infinitively far perspective
	double xs = (i - w_2) * PIXEL_SIZE, ys = (j - h_2) * PIXEL_SIZE;

	return {pos + ex * xs + ey * ys, ez};
}
