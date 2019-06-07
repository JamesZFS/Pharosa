//
// Created by James on 2019/6/7.
//

#ifndef PHAROSA_PTE_H
#define PHAROSA_PTE_H


#include "PathTracing.h"

// Path Tracing Explicit version
class PTE : public PathTracing
{
private:
	Color _radiance(const Ray &ray, size_t depth, bool flag = true) const;    // ** main algorithm **
	// flag - whether to consider self emission term in current layer

public:
	PTE(Scene &scene_, size_t max_depth_ = 5);

	Color radiance(const Ray &ray) const override
	{ return _radiance(ray, 0, true); }

	String info() const override;
};


#endif //PHAROSA_PTE_H
