//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_ILLUMINATOR_H
#define PHAROSA_ILLUMINATOR_H

#include "../lib.h"
#include "../Ray.hpp"
#include "../camera/All.h"
#include "../scene/Stage.h"

// standard illumination algorithm, base class, can access stage and camera
class Illuminator
{
protected:
	Stage &stage;
	Cameras::Camera &camera;

	// interfaces:
	virtual Color radiance(const Ray &ray, unsigned int depth) const = 0;    // ** main algorithm **

public:
	Illuminator(Stage &stage_, Cameras::Camera &camera_);

	virtual ~Illuminator() = default;

	// ** rendering pipeline **
	// do the rendering for n_epoch times
	virtual void render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir);

	virtual void renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
							   unsigned int verbose_step, const String &checkpoint_dir);
};

#include "Illuminator.cpp"

#endif //PHAROSA_ILLUMINATOR_H
