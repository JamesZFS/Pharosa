//
// Created by James on 2019/4/5.
//

#include "ImageMixer.h"

ImageMixer::ImageMixer(size_t width_, size_t height_) : Camera(Pos::ORIGIN, ElAg::NONROT, width_, height_)
{
}

Ray ImageMixer::shootRayAt(double i, double j) const
{
	warn("Error: invalid use of ImageMixer::shootRayAt.\n");
	exit(1);
}
