//
// Created by James on 2019/4/22.
//

#ifndef PHAROSA_OBJPARSER_H
#define PHAROSA_OBJPARSER_H

#include "../../defs.h"
#include "../../core/Vec.h"
#include "../../scene/Object.h"

namespace Parser
{
	ObjectList *parseObjFile(const String &obj_path, double scale, const TransMat &trans_mat,
							 const Material *material);    // load mesh segments from objects file
}

#endif //PHAROSA_OBJPARSER_H
