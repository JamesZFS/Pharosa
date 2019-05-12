//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_DOFCAMERA_H
#define PHAROSA_DOFCAMERA_H

#include "Camera.h"

// depth of field camera
class DOFCamera : public Camera
{
public:
	const double length, focus, aperture;	// length of camera, distance to focus plane, random circle range

	DOFCamera(const Json &json);

	Ray shootRayAt(double i, double j) const override;
};

#endif //PHAROSA_DOFCAMERA_H
