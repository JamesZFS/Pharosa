//
// Created by James on 2019/6/7.
//

#ifndef PHAROSA_PTF_H
#define PHAROSA_PTF_H

#include "PathTracing.h"

// Path Tracing Forward version
class PTF : public PathTracing
{
public:
	PTF(const Scene &scene_, Camera &camera_, size_t max_depth_ = 5);

	Color radiance(const Ray &ray) const override;

	String info() const override;
};


#endif //PHAROSA_PTF_H
