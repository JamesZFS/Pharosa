//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_BASICCAMERA_H
#define PHAROSA_BASICCAMERA_H

#include "Camera.h"

class BasicCamera : public Camera
{
public:
	BasicCamera(const Pos &pos_, const ElAg &euler_angles_,
				unsigned int width_ = 1024, unsigned int height_ = 768, double fovx_ = M_PI_4);

	Ray shootRayAt(unsigned int i, unsigned int j) const override;
};

#include "BasicCamera.cpp"

#endif //PHAROSA_BASICCAMERA_H
