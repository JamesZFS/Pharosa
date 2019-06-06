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
	Color _radiance(const Ray &ray, size_t depth, bool flag = true) const;    // ** main algorithm **
	// flag - whether to consider self emission term in current layer

public:
	const size_t max_depth;

	PathTracing(Scene &scene_, size_t max_depth_ = 5);	// by default - depth = 5

	Color radiance(const Ray &ray) const override
	{ return _radiance(ray, 0); }

	String info() const override;
};

#endif //PHAROSA_PATHTRACING_H
