//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_IMAGEMIXER_H
#define PHAROSA_IMAGEMIXER_H

#include "Camera.h"

// mix several ppm files together todo re-implement this
class ImageMixer : public Camera
{
public:
	ImageMixer(size_t width_ = 1024, size_t height_ = 768);

	Ray shootRayAt(real i, real j) const override;
};

#endif //PHAROSA_IMAGEMIXER_H
