//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_RAYTRACING_H
#define PHAROSA_RAYTRACING_H

#include "Algorithm.h"

// Ray Tracing algorithm
class RayTracing : public Algorithm
{
protected:
	Color radiance(const Ray &ray, size_t depth) const override;    // ** main algorithm **

public:
	const size_t max_depth;

	RayTracing(Scene &scene_, Camera &camera_, size_t max_depth_ = 5);	// by default - depth = 5

	void report() override;
};

#endif //PHAROSA_RAYTRACING_H
