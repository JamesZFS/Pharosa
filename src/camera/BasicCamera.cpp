//
// Created by James on 2019/4/2.
//

#include "BasicCamera.h"

BasicCamera::BasicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_) :
		Camera(pos_, euler_angles_, width_, height_), cur_i(0), cur_j(0), w_2(width_ >> 1), h_2(height_ >> 1)
{
}

const Ray &BasicCamera::shootRay()
{
	// convert screen pixel crd sys (i, j) to screen center crd sys (x', y', z')
	// using (x', y') = (i - w/2, j - h/2) * a
	double xs = (cur_i - w_2) * PIXEL_SIZE, ys = (cur_j - h_2) * PIXEL_SIZE;

	// from screen center crd (x', y') get global crd of ray.dir
	// using dir = x' ex + y' ey + ez * CAM_FOCUS
	cur_ray.dir = ex * xs + ey * ys + ez * CAMERA_FOCUS;
	cur_ray.dir.norm(); // todo

	// calculate ray.org
	// solve t ez.dir = F ez.ez
	double t = CAMERA_FOCUS / (ez % cur_ray.dir);
	assert(t > EPS); // todo
	cur_ray.org = this->pos + cur_ray.dir * t;

	// update progress, first ++x, then ++y
	if (++cur_i == width) {
		cur_i = 0;
		if (++cur_j > height) {
			warn("improper use of shootRay, pixel index overflows.");
		}
	}

	return cur_ray;
}

double BasicCamera::progress() const
{
	return (cur_j * width + cur_i) * 1.0 / size;
}

void BasicCamera::resetProgress()
{
	cur_i = cur_j = 0;
}
