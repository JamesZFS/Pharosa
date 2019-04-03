//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n");
	using namespace Scenes;
	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	Scenes::Object *p[] = {
			new Sphere(Pos(0, 0, 100), 50, Color::RED, Emission::LIGHTSOURCE, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 0, -100), 20, Color::BLUE, Emission::DARK, ElAg::NONROT, Object::REFL),
			new Sphere(Pos(0, 100, 0), 30, Color::GREEN, Emission::DARK, ElAg::NONROT, Object::REFR),
//			new Triangle(Pos::ORIGIN, new Pos[3]{{0, -100, 0}, {0, 100, 0}, {0, 0, 50}}, Color::WHITE, Emission::DARK, ElAg::NONROT, Object::DIFF),
			new Triangle(Pos(-80, 0, 0), new Pos[3]{{0, -1000, 0}, {0, 1000, 2000}, {0, 1000, -2000}}, Color::WHITE, Emission::DARK, ElAg::NONROT, Object::DIFF),
			new Triangle(Pos(0, 0, -300), new Pos[3]{{0, -1000, 0}, {0, 1000, 2000}, {0, 1000, -2000}}, Color::WHITE, Emission::DARK, ElAg(M_PI_2, M_PI_2, M_PI_2), Object::DIFF),
	};

	renderer.setupStage(ObjectGroup(p, p + 5));

//	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500, "../out/fun.ppm", 115);
	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500);

//	renderer.start(100, 10000, "../out/fun/");
	renderer.start(100, 10000);

	renderer.save("../out/fun-100.ppm");

	debug("\n");
	return 0;
}
