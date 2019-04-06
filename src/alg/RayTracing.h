//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_RAYTRACING_H
#define PHAROSA_RAYTRACING_H

#include "Illuminator.h"

// Ray Tracing algorithm
class RayTracing : public Illuminator
{
protected:
	Color radiance(const Ray &ray, unsigned int depth) const;    // ** main algorithm **

public:
	RayTracing(Stage &stage_, Cameras::Camera &camera_);

};

#include "RayTracing.cpp"

#endif //PHAROSA_RAYTRACING_H
