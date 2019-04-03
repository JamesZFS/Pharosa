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

	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
	renderer.setupCamera(Pos::ORIGIN, ElAg::NONROT);
	renderer.start(1);
	renderer.save("../out/hhh");

	debug("\n\n");
	return 0;
}
