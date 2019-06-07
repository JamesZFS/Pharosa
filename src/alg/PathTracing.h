//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_PATHTRACING_H
#define PHAROSA_PATHTRACING_H

#include "Algorithm.h"

// Ray Tracing algorithm
class PathTracing : public Algorithm
{
protected:
	const static size_t DEPTH_LIMIT;	// in case not encounter stack overflow, but possibly introduce bias

	const size_t max_depth;

	Color _radiance(const Ray &ray, size_t depth) const;

public:
	PathTracing(Scene &scene_, size_t max_depth_ = 5);	// by default - depth = 5

	Color radiance(const Ray &ray) const override
	{ return _radiance(ray, 0); }

	String info() const override;
};

#endif //PHAROSA_PATHTRACING_H
