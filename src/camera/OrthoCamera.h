//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_ORTHOCAMERA_H
#define PHAROSA_ORTHOCAMERA_H

#include "Camera.h"

class OrthoCamera : public Camera
{
public:
	OrthoCamera(const Json &json);

	Ray shootRayAt(real i, real j) const override;
};

#endif //PHAROSA_ORTHOCAMERA_H
