//
// Created by James on 2019/4/5.
//

#include "ImageMixer.h"

ImageMixer::ImageMixer(unsigned int width_, unsigned int height_) : Camera(Pos::ORIGIN, ElAg::NONROT, width_, height_,
																		   0)
{
}

Ray ImageMixer::shootRayAt(unsigned int i, unsigned int j) const
{
	warn("Error: invalid use of ImageMixer::shootRayAt.\n");
	exit(1);
}