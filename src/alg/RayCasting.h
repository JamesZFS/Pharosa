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
	RayCasting(Stage &stage_, Camera &camera_);
	
	static Dir LIGHT_DIR;	// parallel light orientation
};

#endif //PHAROSA_DIRECTIMAGING_H
