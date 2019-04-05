//
// Created by James on 2019/4/5.
//

#include "ImageMixer.h"

ImageMixer::ImageMixer(unsigned int width_, unsigned int height_) : Camera(Pos::ORIGIN, ElAg::NONROT, width_, height_)
{
}

const Ray &ImageMixer::shootRay()
{
	warn("Warning: invalid use of ImageMixer::shootRay.\n");
	return cur_ray;
}
