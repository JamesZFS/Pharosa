//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_BASICCAMERA_H
#define RAYTRACKER_BASICCAMERA_H

#include "Camera.h"

class BasicCamera : public Camera
{
private:
	double w_2, h_2;            // width / 2, height / 2

public:
	BasicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_);

	const Ray &shootRay() override;
};

#include "BasicCamera.cpp"

#endif //RAYTRACKER_BASICCAMERA_H
