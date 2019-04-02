//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_GI_H
#define RAYTRACKER_GI_H

#include "../lib.h"
#include "../camera/All.h"
#include "../scene/Stage.h"

// standard global illumination algorithm, base class
class GI
{
private:
	const Stage &stage;    // cannot change stage
	Cameras::BasicCamera &camera;
public:
	GI(const Stage &stage_, Cameras::BasicCamera &camera_);
};

#include "GI.cpp"

#endif //RAYTRACKER_GI_H
