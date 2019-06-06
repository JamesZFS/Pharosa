//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_BASICCAMERA_H
#define PHAROSA_BASICCAMERA_H

#include "Camera.h"

//template <real PX = 1.0>
class BasicCamera : public Camera
{
public:
	const real length;
	// cache:
	const Pos ez_length, pos_ez_length;	// ez * length, c + ez * length

	BasicCamera(const Json &json);

	Ray shootRayAt(real i, real j) const override;
};

#endif //PHAROSA_BASICCAMERA_H
