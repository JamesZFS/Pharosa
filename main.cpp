/** Created by James on 2019/4/1.
 *  Pharosa, an individual 3D rendering engine based on state-of-the-art ray tracing algorithms
 *  Author: Fengshi Zheng
 *  License: Department of Computer Science and Technology, Tsinghua University ®
 */

#include "src/Renderer.h"

using namespace std;
using namespace Scenes;

int main()
{
	// init random engine
	Funcs::generator.seed((unsigned int) time(nullptr));

	Object *p[] = {
	new Sphere(Pos(1e5 + 1, 40.8, 81.6),   1e5,  Color(.75, .25, .25), Emission::NONE, Object::DIFF),//Left
	new Sphere(Pos(-1e5 + 99, 40.8, 81.6), 1e5,  Color(.25, .25, .75), Emission::NONE, Object::DIFF),//Rght
	new Sphere(Pos(50, 40.8, 1e5),         1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Back
//	new Sphere(Pos(50, 40.8, -1e5 + 170),  1e5,  Color(),              Emission::NONE, Object::DIFF),//Frnt
	new Sphere(Pos(50, 1e5, 81.6),         1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Botm
	new Sphere(Pos(50, -1e5 + 81.6,81.6),  1e5,  Color(.75, .75, .75), Emission::NONE, Object::DIFF),//Top
	new Sphere(Pos(27, 16.5, 47),          16.5, Color::WHITE * .999,  Emission::NONE, Object::SPEC),//Mirr
	new Sphere(Pos(73, 16.5, 78),          16.5, Color::WHITE * .999,  Emission::NONE, Object::REFR),//Glas
	new Sphere(Pos(50, 681.6 - .27, 81.6), 600,  Color::BLACK,   Emission(12, 12, 12), Object::DIFF) //Lite
	};

	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
	renderer.setupStage();
	renderer.stage().fromObjectList(ObjectList(p, p + 8));

//	renderer.setupCamera(Pos(50, 50, 266.6), ElAg(0, M_PI - 3 DEG, 0));
	renderer.setupCamera(Pos(50, 50, 200.6), ElAg(0, M_PI - 3 DEG, 0), 800, 500);
//						 "out/omp/100 - static - inside.ppm", 100);
//	renderer.setupCamera(Pos(50, 25, 150), ElAg(0, M_PI, 0));
	//	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 600, 400, "out/Mesh Object Test - 50.ppm", 50);

	//	renderer.start(100, 10000, "out/fun/");
	renderer.start(20, 0);

	renderer.save("out/omp/F - 20.ppm");

	return 0;
}
