/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.hpp"
#include "test.cpp"

using namespace std;

Sphere *ball;

void motion()
{
	ball->translate({-1, 1.0, -0.3});
}

void Pharosa(int argc, char *argv[])
{
	unsigned int n_epoch = (argc >= 2) ? (unsigned int) atoi(argv[1]) : 10;
	String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
	ball = new Sphere(10, Pos(73, 10, 78), Color::WHITE * .999, Emission::NONE, ElAg(), Object::REFR);

	Object *p[] = {
			new Sphere(1e5, Pos(1e5 + 1, 40.8, 81.6), Color(.75, .25, .25), Emission::NONE, ElAg(), Object::DIFF),//Left
//			new Sphere(1e5, Pos(-1e5 + 99, 40.8, 81.6), Color(.25, .25, .75), Emission::NONE, ElAg(),
//					   Object::DIFF),//Rght
			new Sphere(1e5, Pos(50, 40.8, 1e5), Color(.75, .75, .75), Emission::NONE, ElAg(), Object::DIFF),//Back
			new Sphere(1e5, Pos(50, 1e5, 81.6), Color(.75, .75, .75), Emission::NONE, ElAg(), Object::DIFF),//Botm
			new Sphere(1e5, Pos(50, -1e5 + 81.6, 81.6), Color(.75, .75, .75), Emission::NONE, ElAg(), Object::DIFF),//Top
			new Sphere(8, Pos(27, 16.5, 100), Color(0.2, 0.4, 0.8), Emission::NONE, ElAg(), Object::SPEC),//Mirr
			ball,    //Glas
//			new Triangle(new Pos[3]{Pos(20, 0, 100), {80, 0, 100}, {50, 80, 80}}, Pos::ORIGIN, {0.6, 0.4, 0.5}, Emission::NONE,
//						 ElAg::NONROT, Object::REFR),
			new Cube(new Dir[3]{Dir::X_AXIS, Dir::Y_AXIS, Dir::Z_AXIS},
					 new Pos[3][2]{
							 {{0, 0, 0}, {30, 0,  0}},
							 {{0, 0, 0}, {0,   30, 0}},
							 {{0, 0, 0}, {0,   0,  30}}}, Pos(10, 0, 0),
					 Color(0.5, 0.5, 0.8)),
			new Sphere(600, Pos(50, 681.6 - .27, 81.6), Color::BLACK, Emission(12, 12, 12), ElAg(), Object::DIFF) //Lite
	};

	RayCasting::LIGHT_DIR = Dir(0.5, -0.5, -1);

	Renderer<RayTracing<>, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromObjectList(ObjectList(p, p + sizeof(p) / sizeof(Object *)));

	renderer.setupCamera(Pos(200, 60, 280), ElAg(0, M_PI, -45 * DEG), 1024, 768);
	renderer.camera().translate({0, 0, -70});

	renderer.start(n_epoch, 0);
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	renderer.save(out_path);
}

int main(int argc, char *argv[])
{
	Pharosa(argc, argv);
	return 0;
}
