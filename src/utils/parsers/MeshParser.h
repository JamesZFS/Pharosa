//
// Created by James on 2019/4/22.
//

#ifndef PHAROSA_MESHPARSER_H
#define PHAROSA_MESHPARSER_H

#include "../../Pharosa.h"
#include "../../core/Vec.h"
#include "../../scene/Object.h"

namespace Parser
{
	ObjectList *parseMeshes(const String &obj_path, double scale, const TransMat &trans_mat,
							const Material *material);    // load mesh segments from objects file
}

#endif //PHAROSA_MESHPARSER_H
