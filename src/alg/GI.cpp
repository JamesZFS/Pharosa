//
// Created by James on 2019/4/2.
//

#include "GI.h"


GI::GI(const Stage &stage_, Cameras::Camera &camera_) : stage(stage_), camera(camera_)
{
}

void GI::render(unsigned int n_epoch)
{
	warn("Warning: method GI::render is not implemented.\n");
}
