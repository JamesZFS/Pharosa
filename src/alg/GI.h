//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_GI_H
#define RAYTRACKER_GI_H

#include "../lib.h"
#include "../Ray.hpp"
#include "../camera/All.h"
#include "../scene/Stage.h"

// standard global illumination algorithm, base class
class GI
{
protected:
	const Stage &stage;    // cannot change stage
	Cameras::Camera &camera;
public:
	GI(const Stage &stage_, Cameras::Camera &camera_);

	virtual ~GI() = default;

	// interfaces:
	virtual void render(unsigned int n_epoch) = 0;    // do the rendering for n_epoch times
};

#include "GI.cpp"

#endif //RAYTRACKER_GI_H
