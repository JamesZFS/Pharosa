/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */


#include "src/utils/funcs.hpp"
#include "src/Renderer.h"
#include <iostream>

using namespace std;

int main()
{
	// todo implement superposition of multiple .ppm
	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
//	Funcs::generator.seed(1);

	debug("\n");
	using namespace Scenes;
	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;

	Scenes::Object *p[] = {
			new Sphere(Pos(0, 0, 200), 50, Color::RED, Emission::SPLENDID, ElAg::NONROT, Object::DIFF),
			new Sphere(Pos(0, 300, -200), 100, Color::BLUE, Emission::NONE, ElAg::NONROT, Object::REFL),
			new Sphere(Pos(0, -100, 0), 80, Color::GREEN, Emission::NONE, ElAg::NONROT, Object::REFR),
			new InfPlane(Pos(0, 0, 300), Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),	// ceil
			new InfPlane(Pos(0, 0, -300), Color::YELLOW, Emission::NONE, ElAg::NONROT, Object::DIFF), // floor
			new InfPlane(Pos(-200, 0, 0), Color::WHITE, Emission::NONE, ElAg(M_PI_2, M_PI_2), Object::DIFF),	// back
			new InfPlane(Pos(0, -500, 0), Color::RED, Emission::NONE, ElAg(0, M_PI_2), Object::REFL),	// left
			new InfPlane(Pos(0, 500, 0), Color::BLUE, Emission::NONE, ElAg(0, M_PI_2), Object::REFL)	// right
	};

	renderer.setupStage(ObjectGroup(p, p + 8));

//	renderer.setupCamera(Pos(800, 300, 0), ElAg(2.0 * M_PI / 3.0, -M_PI_2, 0), 800, 500, "../out/Cornell Box - 50.ppm", 50);
	renderer.setupCamera(Pos(800, 300, 0), ElAg(120 DEG, -90 DEG, 0), 800, 500);

//	renderer.start(100, 10000, "../out/fun/");
	renderer.start(5);

	renderer.save("../out/Mesh Object Test - 5.ppm");

	debug("\n");
	return 0;
}
