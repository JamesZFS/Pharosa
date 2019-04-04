//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_RAYTRACING_H
#define RAYTRACKER_RAYTRACING_H

#include "GI.h"

// Ray Tracing algorithm
class RayTracing : public GI
{
private:
	// render with progressbar
	void renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
					   unsigned int verbose_step, const String &checkpoint_dir);

protected:
	Color radiance(const Ray &ray, unsigned int depth) const;    // ** main algorithm **

public:
	RayTracing(const Stage &stage_, Cameras::Camera &camera_);

	// ** rendering pipeline **
	void render(unsigned int n_epoch, unsigned int prev_epoch, unsigned int verbose_step,
				const String &checkpoint_dir) override;
};

#include "RayTracing.cpp"

#endif //RAYTRACKER_RAYTRACING_H
