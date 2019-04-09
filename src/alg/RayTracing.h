//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_RAYTRACING_H
#define PHAROSA_RAYTRACING_H

#include "Algorithm.h"

// Ray Tracing algorithm
template<unsigned int MAX_DEPTH = 5>	// depth limit to use R.R.
class RayTracing : public Algorithm
{
protected:
	Color radiance(const Ray &ray, unsigned int depth) const override;    // ** main algorithm **

public:
	RayTracing(Stage &stage_, Camera &camera_);

};

#include "RayTracing.tcc"

#endif //PHAROSA_RAYTRACING_H
