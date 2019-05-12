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
	const double length;

	BasicCamera(const Json &json);

	Ray shootRayAt(double i, double j) const override;
};

#endif //PHAROSA_BASICCAMERA_H
