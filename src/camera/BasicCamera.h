//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_BASICCAMERA_H
#define PHAROSA_BASICCAMERA_H

#include "Camera.h"

//template <double PX = 1.0>
class BasicCamera : public Camera
{
public:
	BasicCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	Ray shootRayAt(double i, double j, double sigma = 0) const override;

	static double PIXEL_SIZE, FOCUS;
};

#include "BasicCamera.cpp"

#endif //PHAROSA_BASICCAMERA_H
