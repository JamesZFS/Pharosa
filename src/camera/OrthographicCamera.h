//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_ORTHOGRAPHICCAMERA_H
#define PHAROSA_ORTHOGRAPHICCAMERA_H

#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const Pos &pos_, const ElAg &euler_angles_,
					   unsigned int width_ = 1024, unsigned int height_ = 768);

	Ray shootRayAt(double i, double j, double sigma) const override;

	Ray shootRayAt(double i, double j) const override;

	static double PIXEL_SIZE;
};

#endif //PHAROSA_ORTHOGRAPHICCAMERA_H
