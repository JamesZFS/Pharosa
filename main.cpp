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
size_t __print_cnt__ = 0;

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

	// ********************************************* define stage *********************************************
	ball = new Object(Sphere(10, Pos(-25, -65, -10)), Color::WHITE * 0.98, Emission::NONE, Object::REFR);

	ObjectList singletons{
			new Object(Sphere(600, Pos(0, 678, 0), ElAg()), Color(), Emission::SPLENDID), //Lite
			new Object(InfPlane(Dir(0, 1, 0), Pos(0, -80, 0)), Color::BROWN),//Botm brown
			new Object(InfPlane(Dir(0, -1, 0), Pos(0, 82, 0)), Color(0.9, 0.9, 0.9)),//ceil
			new Object(InfPlane(Dir(1, 0, 0), Pos(-90, 0, 0)), Color(214, 69, 69) / 255.),//left red
			new Object(InfPlane(Dir(1, 0, 0), Pos(90, 0, 0)), Color(64, 77, 255) / 255.),//right blue
			new Object(InfPlane(Dir(0, 0, 1), Pos(0, 0, -200)), Color(0.8, 0.8, 0.8), Emission(), Object::DIFF),	// front gray
			ball,
//			new Object(Sphere(100, Pos(0, 0, 0)), Color::WHITE, Emission(), Object::DIFF),	// outter
//			cube,
//			new Object(Sphere(3, Pos(0, 0, 0)), Color(0.2, 0.8, 1.0), Emission::NONE, Object::SPEC),	// origin
//			ball,    //Glas
//			triangle,
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
//	auto box = new BoundingCube(box_objs);
	auto box = new BoundingCube();
	box->translate({-20, -80, -40}).rotate({0, 0, 30 * DEG});
	box->fromObjFile("res/dragon0_1 - done.obj", 40, Color(178, 255, 110) / 255.);
	warn("box size: " << box->objects.size() << "\n");

	// ********************************************* init render engine *********************************************
	RayCasting::LIGHT_DIR = Dir(-0.2, -1, +0.3);

//	Renderer<RayTracing<5>, BasicCamera> renderer;
	Renderer<RayTracing<3>, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromList(singletons);
	renderer.stage().append(box);

	renderer.setupCamera(Pos(0, 0, 280), ElAg(0, 176 * DEG, 0), 1024, 768);
//	renderer.camera().rotate(ElAg(0, -5 * DEG, 0));
//	renderer.camera().translate({0, 0, -70});

	// ********************************************* start rendering *********************************************
	renderer.start(n_epoch, 1, "out/dragon");
//	renderer.start(n_epoch, 1);
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	// ********************************************* save results *********************************************
	renderer.save(out_path);
}

int main(int argc, char *argv[])
{
	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
	Pharosa(argc, argv);
//	Test::matrix();
	return 0;
}
