//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_RAYTRACING_H
#define RAYTRACKER_RAYTRACING_H

#include "GI.h"

// Ray Tracing algorithm
class RayTracing : public GI
{
public:
	RayTracing(const Stage &stage_, Cameras::BasicCamera &camera_);

	void render(unsigned int n_epoch) override;

protected:
	Color radiance(const Ray &r, unsigned int depth);	// main alg
};

#include "RayTracing.cpp"

#endif //RAYTRACKER_RAYTRACING_H
