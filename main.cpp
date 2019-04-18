/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.hpp"
#include "test.cpp"

using namespace std;

Object *ball;

void motion()
{
	ball->geo->translate({-1, 1.0, -0.3});
}

void Pharosa(int argc, char *argv[])
{
	unsigned int n_epoch = (argc >= 2) ? (unsigned int) atoi(argv[1]) : 10;
	String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
	ball = new Object(new Sphere(10, Pos(73, 10, 78), ElAg(0, 0, 0)), Color::WHITE * .999, Emission::NONE, Object::REFR);

	Object *p[] = {
			new Object(new InfPlane(Dir(0, 1, 0), Pos(0, -10, 0))),//Botm
			new Object(new InfPlane(Dir(1, 0, 0), Pos(-20, 0, 0))),//left
			new Object(new InfPlane(Dir(0, 0, 1), Pos(0, 0, -30))),//right
			new Object(new InfPlane(Dir(0, -1, 0), Pos(0, 81.6, 0))),//ceil
			new Object(new Sphere(8, Pos(27, 16.5, 100), Color(0.2, 0.4, 0.8)), Emission::NONE, ElAg(), Object::SPEC),
			ball,    //Glas
			new Object(new Triangle(new Pos[3]{{0, 0, 80}, {80, 0, 0}, {0, 80, 0}}, Pos(50, 0, 50), {0.8, 0.6, 0.5}),
					Emission(), ElAg(), Object::REFR),
			new Object(new Cube(new Dir[3]{Dir::X_AXIS, Dir::Y_AXIS, Dir::Z_AXIS},
					 new Pos[3][2]{
							 {{0, 0, 0}, {30, 0,  0}},
							 {{0, 0, 0}, {0,  30, 0}},
							 {{0, 0, 0}, {0,  0,  50}}}, Pos(10, 30, 20), ElAg(0, 10 * DEG, 0)),
					 Color(0.5, 0.5, 0.8), Emission::NONE),
			new Object(new Sphere(600, Pos(50, 681.6 - .27, 81.6), ElAg()), Color::BLACK, Emission(12, 12, 12), Object::DIFF) //Lite
	};

	RayCasting::LIGHT_DIR = Dir(0.5, -0.5, -1);

	Renderer<RayCasting, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromObjectList(ObjectList(p, p + sizeof(p) / sizeof(Object *)));

	renderer.setupCamera(Pos(200, 60, 280), ElAg(0, M_PI, -45 * DEG), 1024, 768);
	renderer.camera().rotate(ElAg(0, -5 * DEG, 0));
	renderer.camera().translate({0, 0, -70});

	renderer.start(n_epoch, 0, "out/checkpoints");
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	renderer.save(out_path);
}

int main(int argc, char *argv[])
{
//	Pharosa(argc, argv);
	Test::matrix();
	return 0;
}
