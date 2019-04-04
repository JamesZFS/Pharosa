//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include <iostream>

using namespace std;

// todo rename this project to Pharosa ( variation of pharos - lighthouse )

int main()
{
	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));

	debug("\n");
	using namespace Scenes;
	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	Scenes::Object *p[] = {
			new Sphere(Pos(0, 0, 100), 50, Color::RED, Emission::BRIGHT, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 0, -100), 20, Color::BLUE, Emission::NONE, ElAg::NONROT, Object::REFL),
			new Sphere(Pos(0, 100, 0), 30, Color::GREEN, Emission::NONE, ElAg::NONROT, Object::REFR),
//			new Triangle(Pos::ORIGIN, new Pos[3]{{0, -100, 0}, {0, 100, 0}, {0, 0, 50}}, Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),
			new Triangle(Pos(-80, 0, 0), new Pos[3]{{0, -1000, 0}, {0, 1000, 2000}, {0, 1000, -2000}}, Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),
			new Triangle(Pos::ORIGIN, new Pos[3]{{100, -2000, -200}, {0, 1000, -200}, {500, 1000, -200}}, Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),
	};

	renderer.setupStage(ObjectGroup(p, p + 5));

//	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500, "../out/fun.ppm", 115);
	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500);

//	renderer.start(100, 10000, "../out/fun/");
	renderer.start(100, 10000);

	renderer.save("../out/fun-100+.ppm");

	debug("\n");
	return 0;
}
