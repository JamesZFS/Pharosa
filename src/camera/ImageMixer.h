//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_IMAGEMIXER_H
#define PHAROSA_IMAGEMIXER_H

#include "Camera.h"

// mix several ppm files together
class ImageMixer : public Camera
{
public:
	ImageMixer(unsigned int width_ = 1024, unsigned int height_ = 768);

	Ray shootRayAt(double i, double j, double sigma) const override;

	Ray shootRayAt(double i, double j) const override;
};

#endif //PHAROSA_IMAGEMIXER_H
