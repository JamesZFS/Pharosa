//
// Created by James on 2019/4/5.
//

#include "ImageMixer.h"

ImageMixer::ImageMixer(unsigned int width_, unsigned int height_) : Camera(Pos::ORIGIN, ElAg::NONROT, width_, height_)
{
}

Ray ImageMixer::shootRayAt(double i, double j, double sigma) const
{
	warn("Error: invalid use of ImageMixer::shootRayAt.\n");
	exit(1);
}
