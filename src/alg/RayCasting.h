//
// Created by James on 2019/4/6.
//

#ifndef PHAROSA_DIRECTIMAGING_H
#define PHAROSA_DIRECTIMAGING_H

#include "Algorithm.h"

// render directly the RGB color of an object
class RayCasting : public Algorithm
{
protected:
	Color radiance(const Ray &ray, size_t depth) const override;

public:
	const Dir light_dir;	// parallel light orientation

	RayCasting(Scene &scene_, const Dir &light_dir_ = Dir(-1, -1, -1));

	String info() override;
};

#endif //PHAROSA_DIRECTIMAGING_H
