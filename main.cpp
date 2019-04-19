/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.hpp"
#include "test.cpp"

using namespace std;

#ifdef __DEV_STAGE__
size_t __counter__ = 0;

#endif

Object *ball;

void motion()
{
	ball->geo->translate({-1, 1.0, -0.3});
}

void Pharosa(int argc, char *argv[])
{
	// command line args:
	size_t n_epoch = (argc >= 2) ? (size_t) atoi(argv[1]) : 10;
	String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
	ball = new Object(Sphere(10, Pos(73, 10, 78)), Color::WHITE * .999, Emission::BRIGHT, Object::DIFF);

	auto *front = new Object(InfPlane(Dir(0, 0, 1), Pos(0, 0, 200)), Color::YELLOW);//front
	auto *right = new Object(InfPlane(Dir(1, 0, 0), Pos(90, 0, 0)));//right
	auto *cube = new Object(
			Cube({{Pos(20, 0, 0), Pos(0, 20, 0), Pos(0, 0, 60)}}, Pos(30, -30, -40), ElAg(45 * DEG, -10 * DEG, 10 * DEG)),
			Color(0.5, 0.5, 0.8), Emission::NONE
	);
	cube->geo->translate({0, 0, -20}).rotate({10 * DEG, 0, 0}).translate({-50})
			.rotate({-10 * DEG}).rotate({0, 0, -10 * DEG}).rotate({0, 10 * DEG}).rotate({-45 * DEG});

	warn("\nfront position:\n");
	front->geo->mat.report();

	Object *p[] = {
			new Object(InfPlane(Dir(0, 1, 0), Pos(0, -80, 0)), Color::RED * 0.5),//Botm
			new Object(InfPlane(Dir(1, 0, 0), Pos(-90, 0, 0)), Color::BLUE * 0.5),//left
			new Object(InfPlane(Dir(0, -1, 0), Pos(0, 82, 0))),//ceil
			right,
			front,
			cube,
			new Object(Sphere(10, Pos(0, 0, 10)), Color(0.2, 0.8, 1.0), Emission::NONE, Object::SPEC),
			ball,    //Glas
			new Object(Triangle({{
										 {0, 0, 0},
										 {30, 0, 0},
										 {0, 75, 0}
								 }}, Pos(0, 0, 10), ElAg()), {0.8, 0.6, 0.5},
					   Emission(), Object::REFR),
			new Object(Sphere(600, Pos(50, 681.6 - .27, 81.6), ElAg()), Color::BLACK, Emission(12, 12, 12),
					   Object::DIFF) //Lite
	};

	RayCasting::LIGHT_DIR = Dir(-0.2, -1, +0.3);

	Renderer<RayCasting, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromObjectList(ObjectList(p, p + sizeof(p) / sizeof(Object *)));

	renderer.setupCamera(Pos(0, 0, -200), ElAg(M_PI, 0, 0), 1024, 768);
//	renderer.camera().rotate(ElAg(0, -5 * DEG, 0));
//	renderer.camera().translate({0, 0, -70});

	renderer.start(n_epoch, 0, "out/checkpoints");
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	renderer.save(out_path);
}

int main(int argc, char *argv[])
{
	Pharosa(argc, argv);
//	Test::matrix();
	return 0;
}
