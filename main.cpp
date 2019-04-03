//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");
	Cameras::BasicCamera basicCamera({0, 0, 0}, {0, 0, 0});
	Cameras::Camera camera({0, 0, 0}, {0, 0, 0});
	Cameras::Camera &cam = basicCamera;

	int cnt = 0;
	while (!cam.finishedDisplay()) {
		++cnt;
		cam.shootRay();
		cam.updateProgress();
	}
	assert(cnt == 1024 * 768);

	debug("\n\n");
	return 0;
}
