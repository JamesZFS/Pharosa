//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Stage stage;
	Cameras::BasicCamera camera(ORIGIN, NONROT);
	Algorithms::RayTracing rt(stage, camera);
	rt.render(1);
//	Renderer renderer(Algorithms::RayTracing, Cameras::BasicCamera);

	debug("\n\n");
	return 0;
}
