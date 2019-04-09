/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.hpp"
//#include "test.cpp"

using namespace std;

Sphere *ball;

void motion()
{
	ball->translate({-1, 1.0, -0.2});
}

int main(int argc, char *argv[])
{
	unsigned int n_epoch = (argc >= 2) ? (unsigned int) atoi(argv[1]) : 10;
	String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));
	ball = new Sphere(Pos(73, 16.5, 78), 16.5, Color::WHITE * .999, Emission::NONE, Object::REFR);

	Object *p[] = {
	new Sphere(Pos(1e5 + 1, 40.8, 81.6),   1e5,  Color(.75, .25, .25), Emission::NONE, Object::DIFF),//Left
	new Sphere(Pos(-1e5 + 99, 40.8, 81.6), 1e5,  Color(.25, .25, .75), Emission::NONE, Object::DIFF),//Rght
	new Sphere(Pos(50, 40.8, 1e5),         1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Back
//	new Sphere(Pos(50, 40.8, -1e5 + 170),  1e5,  Color(),              Emission::NONE, Object::DIFF),//Frnt
	new Sphere(Pos(50, 1e5, 81.6),         1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Botm
	new Sphere(Pos(50, -1e5 + 81.6,81.6),  1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Top
	new Sphere(Pos(27, 16.5, 47),          16.5, Color::WHITE * .999,  Emission::NONE, Object::SPEC),//Mirr
	ball,	//Glas
	new Sphere(Pos(50, 681.6 - .27, 81.6), 600,  Color::BLACK,   Emission(12, 12, 12), Object::DIFF) //Lite
	};

	RayCasting::LIGHT_DIR = Dir(1, -1, -1);

	Renderer<RayTracing<>, BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromObjectList(ObjectList(p, p + 8));

	renderer.setupCamera(Pos(50, 52, 285.6), ElAg(0, M_PI - 2.5 DEG, 0), 1024, 768);

	renderer.start(n_epoch, 0);
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

	renderer.save(out_path);


	return 0;
}
