//
// Created by think on 2019/4/1.
//

#include "src/Render.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Actors::Object ob({0, 0, 0}, {1, 1, 1});

	Actors::Sphere sphere({0, 0, 0}, 2, {1, 0, 1});

	Cameras::Camera cam({0, 0, 0}, {1, 1, 1});

	for (unsigned int i = 0; i < 500; ++i) {
		for (unsigned int j = 0; j < 20; ++j) {
			cam.renderInc(i, j, {1, 0, 0});
			cam.renderInc(j, i, {0, 0, 1});
		}
	}

	cam.postProcess();
	cam.writePPM("../out/test3");

	debug("\n\n");
	return 0;
}
