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
	// ********************************************* parse cmd args *********************************************
	size_t n_epoch = (argc >= 2) ? (size_t) atoi(argv[1]) : 10;
	String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));

	// ********************************************* define stage *********************************************
	ball = new Object(Sphere(10, Pos(73, 10, 78)), Color::WHITE * .999, Emission::SPLENDID, Object::DIFF);
	auto *front = new Object(InfPlane(Dir(0, 0, 1), Pos(0, 0, 200)), Color::YELLOW);//front
	auto *right = new Object(InfPlane(Dir(1, 0, 0), Pos(90, 0, 0)));//right
	/*auto *cube = new Object(
			Cube({{Pos(20, 0, 0), Pos(0, 20, 0), Pos(0, 0, 60)}}, Pos(30, -30, -40),
				 ElAg(45 * DEG, -10 * DEG, 10 * DEG)),
			Color(0.5, 0.5, 0.8), Emission::NONE
	);
	auto *triangle = new Object(Triangle({{Pos(0, 0, 0), {30, 0, 0}, {0, 75, 0}}}, Pos(0, 0, 10), ElAg()),
								Color(0.8, 0.6, 0.5), Emission(), Object::REFR);
	cube->geo->translate({0, 0, -20}).rotate({10 * DEG, 0, 0}).translate({-50})
			.rotate({-10 * DEG}).rotate({0, 0, -10 * DEG}).rotate({0, 10 * DEG}).rotate({-45 * DEG}).rotate({0, 0, 10 * DEG});
	triangle->geo->translate({0, -30, 10}).rotate({-45 * DEG, 0, 180 * DEG});*/

	ObjectList singletons{
			new Object(InfPlane(Dir(0, 1, 0), Pos(0, -80, 0)), Color::RED * 0.5),//Botm
			new Object(InfPlane(Dir(1, 0, 0), Pos(-90, 0, 0)), Color::BLUE * 0.5),//left
			new Object(InfPlane(Dir(0, -1, 0), Pos(0, 82, 0))),//ceil
//			new Object(Sphere(100, Pos(0, 0, 0)), Color::WHITE, Emission(), Object::DIFF),	// outter
			right,
			front,
//			cube,
//			new Object(Sphere(3, Pos(0, 0, 0)), Color(0.2, 0.8, 1.0), Emission::NONE, Object::SPEC),	// origin
//			ball,    //Glas
//			triangle,
			new Object(Sphere(600, Pos(0, 681.6 - 21, 0), ElAg()), Color::BLACK, Emission(1, 1, 1), Object::DIFF), //Lite
	};
//
//	ObjectList box_objs{
//			new Object(Sphere(3, Pos(0, 10, 0)), Color(1.0, 0.2, 0.0), Emission(), Object::REFR),
//			new Object(Sphere(5, Pos(0, 0, 0)), Color(0.5, 0.2, 1.0), Emission(), Object::DIFF),
//			new Object(Sphere(3, Pos(10, 0, 0)), Color(0.5, 0.8, 1.0), Emission(), Object::SPEC),
//			new Object(Sphere(3, Pos(-10, 0, 0)), Color(0.2, 0.9, 0.0), Emission(), Object::DIFF),
//			new Object(Sphere(3, Pos(0, -10, 0)), Color(0.0, 0.9, 0.2), Emission(), Object::DIFF),
//	};
//	auto box = new BoundingSphere(Sphere(13, Pos(0, 0, 0)), box_objs);
//	auto box = new BoundingCube(Cube(20, 20, 10, Pos(-10, -10), ElAg()), box_objs);
	auto box = new BoundingCube();
	box->translate({20, -20, -50});
	box->fromObjFile("res/cube.obj", 10);
	warn("box size: " << box->objects.size());

	// ********************************************* init render engine *********************************************
	RayCasting::LIGHT_DIR = Dir(-0.2, -1, +0.3);

	Renderer<RayTracing<>, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromList(singletons);
	renderer.stage().append(box);

	renderer.setupCamera(Pos(0, 0, -150), ElAg(M_PI, 0, 0), 1024, 768);
//	renderer.camera().rotate(ElAg(0, -5 * DEG, 0));
//	renderer.camera().translate({0, 0, -70});

	// ********************************************* start rendering *********************************************
	renderer.start(n_epoch, 0, "out/checkpoints");
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	// ********************************************* save results *********************************************
	renderer.save(out_path);
}

int main(int argc, char *argv[])
{
	Pharosa(argc, argv);
//	Test::matrix();
	return 0;
}
