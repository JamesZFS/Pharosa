//
// Created by James on 2019/4/6.
//

#ifndef PHAROSA_DIRECTIMAGING_H
#define PHAROSA_DIRECTIMAGING_H

#include "Algorithm.h"

// render directly the RGB color of an object
class RayCasting : public Algorithm
{
public:
	const Dir light_dir;    // parallel light orientation

	RayCasting(const Scene &scene_, Camera &camera_, const Dir &light_dir_ = Dir(-1, -1, -1).unitize());

	Color radiance(const Ray &ray) const override;

	String info() const override;
};

#endif //PHAROSA_DIRECTIMAGING_H
