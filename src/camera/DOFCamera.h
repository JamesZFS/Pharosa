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
	const double length, focus;	// u, f
	const double beta, u_v;		// -y'/y, u+v

	DOFCamera(const Json &json);

	inline Ray shootRayAt(double i, double j, double sigma) const override
	{ return shootRayAt(i, j); }

	Ray shootRayAt(double i, double j) const override;
};

#endif //PHAROSA_DOFCAMERA_H
