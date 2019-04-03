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
	stage.objs.push_back(new Scenes::Sphere({0, 0, 0}, 10, WHITE));
	stage.objs.push_back(new Scenes::Triangle({0, 0, 0}, new Pos[3]{}, WHITE));
	stage.objs.push_back(new Scenes::Object({0, 0, 0}, WHITE));
	for (auto &obj:stage.objs) {
		obj->applyTransform();
	}

//	Cameras::BasicCamera camera({0, 0, 0}, {0, 0, 0});
//	Algorithms::RayTracing rayTracing(stage, camera);

	debug("\n\n");
	return 0;
}
