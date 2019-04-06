//
// Created by James on 2019/4/6.
//

#ifndef PHAROSA_DIRECTIMAGING_H
#define PHAROSA_DIRECTIMAGING_H

#include "Illuminator.h"

// render directly the RGB color of an object
class RayCasting : public Illuminator
{
protected:
	Color radiance(const Ray &ray, unsigned int depth) const override;

public:
	RayCasting(Stage &stage_, Cameras::Camera &camera_);
};

#include "RayCasting.cpp"

#endif //PHAROSA_DIRECTIMAGING_H
