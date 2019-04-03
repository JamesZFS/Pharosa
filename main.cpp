//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	debug("\n\n");
	return 0;
}
