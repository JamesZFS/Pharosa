//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_ORTHOCAMERA_H
#define PHAROSA_ORTHOCAMERA_H

#include "Camera.h"

class OrthoCamera : public Camera
{
public:
	OrthoCamera(const Pos &pos_, const ElAg &euler_angles_, size_t width_ = 1024, size_t height_ = 768,
				double pixel_size_ = 0.1);

	OrthoCamera(const Json &json);

	Ray shootRayAt(double i, double j, double sigma) const override;

	Ray shootRayAt(double i, double j) const override;
};

#endif //PHAROSA_ORTHOCAMERA_H
