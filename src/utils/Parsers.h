//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_PARSERS_H
#define PHAROSA_PARSERS_H

#include "../lib.h"
#include "../scene/Object.h"
#include "../geometric/Triangle.h"

// all parsers
namespace Parser
{

	ObjectList parseObjFile(const String &obj_path, double zoom_ratio, const Color &color,
							const Emission &emi, Object::ReflType reft);    // load mesh segments from objects file

}

#endif //PHAROSA_PARSERS_H
