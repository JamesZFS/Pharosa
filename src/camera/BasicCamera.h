//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_BASICCAMERA_H
#define RAYTRACKER_BASICCAMERA_H

#include "../lib.h"
#include "Camera.h"

class BasicCamera : Camera
{
private:
	unsigned int cur_i, cur_j;	// current pixel to shoot toward, screen coordinate sys
	double w_2, h_2; 			// width / 2, height / 2

public:
	BasicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	const Ray &shootRay() override;

	double progress() override;

	void resetProgress() override;
};

#include "BasicCamera.cpp"

#endif //RAYTRACKER_BASICCAMERA_H
