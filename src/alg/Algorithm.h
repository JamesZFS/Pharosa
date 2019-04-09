//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_ALGORITHM_H
#define PHAROSA_ALGORITHM_H

#include "../lib.h"
#include "../core/Ray.hpp"
#include "../camera/All.h"
#include "../scene/Stage.h"

// standard illumination algorithm, base class, can access stage and camera
class Algorithm
{
private:
	bool *is_edge;	// if each pixel on the screen is edge of an object

	// pre-compute object edges via shooting 4 subpixels for each pixel on screen
	void computeEdgePixels();

protected:
	Stage &stage;
	Camera &camera;

	// interfaces:
	virtual Color radiance(const Ray &ray, unsigned int depth) const = 0;    // ** main algorithm **

	inline Color radiance(const Ray &ray) const	// entrance
	{ return radiance(ray, 0); }

public:
	Algorithm(Stage &stage_, Camera &camera_);

	virtual ~Algorithm();

	// ** rendering pipeline **
	// do the rendering for n_epoch times
	virtual void render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir);

	virtual void renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
							   unsigned int verbose_step, const String &checkpoint_dir);
};

#endif //PHAROSA_ALGORITHM_H
