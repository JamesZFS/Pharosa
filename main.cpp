//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Stage stage;
	Cameras::BasicCamera camera({0, 0, 0}, {0, 0, 0});
	Algorithms::RayTracing rayTracing(stage, camera);

	debug("\n\n");
	return 0;
}
