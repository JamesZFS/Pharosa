//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
//#include "src/utils/json.hpp"
#include <iostream>

using namespace std;
using json = nlohmann::json;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
	renderer.setupStage("../config/1.json");
	renderer.setupCamera(Pos::ORIGIN, ElAg::NONROT);
	renderer.start(3);
	renderer.save("../out/test.ppm");

	debug("\n\n");
	return 0;
}
