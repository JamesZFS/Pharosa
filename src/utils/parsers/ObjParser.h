//
// Created by James on 2019/4/22.
//

#ifndef PHAROSA_OBJPARSER_H
#define PHAROSA_OBJPARSER_H

#include "../../lib.h"
#include "../../core/Vec.h"
#include "../../scene/Object.h"

namespace Parser
{
	ObjectList parseObjFile(const String &obj_path, double zoom_ratio, const Color &color,
							const Emission &emi, Object::ReflType reft);    // load mesh segments from objects file
}

#endif //PHAROSA_OBJPARSER_H
