//
// Created by James on 2019/4/22.
//

#ifndef PHAROSA_TEST_H
#define PHAROSA_TEST_H

#include "lib.h"
#include "Renderer.h"
#include "utils/solvers/Linear.h"

namespace Test
{
	void linear();

	void coordinateConvert();

	void onSurface();

	void intersect();

	void matrix();

	void render(int argc, char *argv[]);
}

#endif //PHAROSA_TEST_H
