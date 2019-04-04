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
//	Funcs::generator.seed((unsigned int) time(nullptr));
	Funcs::generator.seed(1);

	debug("\n");
	using namespace Scenes;
	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	Scenes::Object *p[] = {
			new Sphere(Pos(0, 0, 100), 50, Color::RED, Emission::SPLENDID, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 0, -100), 20, Color::BLUE, Emission::NONE, ElAg::NONROT, Object::REFL),
			new Sphere(Pos(0, 100, 0), 30, Color::GREEN, Emission::NONE, ElAg::NONROT, Object::REFR),
			new InfPlane(Pos(0, 0, -200), Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),
			new InfPlane(Pos(-200, 0, 0), Color::WHITE, Emission::NONE, ElAg(M_PI_2, M_PI_2), Object::DIFF)
	};

	renderer.setupStage(ObjectGroup(p, p + 5));

//	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500, "../out/fun.ppm", 115);
	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 800, 500);

//	renderer.start(100, 10000, "../out/fun/");
	renderer.start(10);

	renderer.save("../out/fun-test-plane.ppm");

	debug("\n");
	return 0;
}
