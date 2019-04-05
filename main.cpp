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

//	Object *p[] = {
//			new Sphere(Pos(0, 0, 300), 50, Color::RED, Emission::SPLENDID, ElAg::NONROT, Object::DIFF),    // light
//			new InfPlane(Pos(0, 0, 300), Color::WHITE, Emission::NONE, ElAg::NONROT, Object::DIFF),    // ceil
//			new InfPlane(Pos(0, 0, -300), Color::YELLOW, Emission::NONE, ElAg::NONROT, Object::DIFF), // floor
//			new InfPlane(Pos(-200, 0, 0), Color::WHITE, Emission::NONE, ElAg(M_PI_2, M_PI_2), Object::DIFF),    // front
//			new InfPlane(Pos(600, 0, 0), Color::BLACK, Emission::NONE, ElAg(M_PI_2, M_PI_2), Object::DIFF),    // back
//			new InfPlane(Pos(0, -500, 0), Color::RED, Emission::NONE, ElAg(0, M_PI_2), Object::SPEC),    // left
//			new InfPlane(Pos(0, 500, 0), Color::BLUE, Emission::NONE, ElAg(0, M_PI_2), Object::SPEC)    // right
//	};
//	MeshObj cube(Pos(0, 100, -200), Color::BLUE, Emission::NONE, ElAg::NONROT, Object::DIFF);    // load mesh object
//	cube.fromObjFile("res/cube.obj", 200);
//
//	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
//
//	renderer.setupStage();
//	renderer.stage().fromObjectList(ObjectList(p, p + 7));
//	renderer.stage().appendMeshes(
//			cube.meshes);        // meshes should be excluded from ObjectList and installed via appendMeshes method
//
//	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 600, 400);
////	renderer.setupCamera(Pos(500, 0, 0), ElAg(M_PI_2, -M_PI_2, 0), 600, 400, "out/Mesh Object Test - 50.ppm", 50);
//
////	renderer.start(100, 10000, "out/fun/");
//	renderer.start(1);
//
////	renderer.save("out/cube - 10.ppm");

	// below is a superposition example
	Cameras::ImageMixer mixer;
	mixer.readPPM("best/image_1k.ppm", 1000);
	mixer.readPPM("best/image6.ppm", 500);
	mixer.writePPM("best/mix.ppm");

	return 0;
}
