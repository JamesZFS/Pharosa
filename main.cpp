//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	using namespace Scenes;
	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	Scenes::Object *p[] = {
			new Sphere(Pos(0, 0, 100), 20, Color::RED, Emission::LIGHTSOURCE, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 0, -100), 20, Color::BLUE, Emission::DARK, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 100, 0), 30, Color::GREEN, Emission::DARK, ElAg::NONROT, Object::REFR),
//			new Triangle(Pos::ORIGIN, new Pos[3]{{0, -40, 0}, {0, 40, 0}, {0, 0, 50}}, Color::WHITE, Emission::DARK, ElAg::NONROT, Object::DIFF),
	};

	renderer.setupStage(ObjectGroup(p, p + 3));

	renderer.setupCamera(Pos(300, 0, 0), ElAg(M_PI_2, -M_PI_2, 0));

	renderer.start(5);

	renderer.save("../out/fun.ppm");

	return 0;
}
