//
// Created by James on 2019/4/9.
//
#include "test.h"
#include "Renderer.h"
#include "geometric/All.h"
#include "utils/solvers/Linear.h"
#include "scene/bounding/BoundingSphere.h"

namespace Test
{

	void linear()
	{
		using namespace std;
		using namespace Linear;

		double A[3][3] = {
				{-2.86854, 0.712385, -2.0962},
				{-1.70362, -4.42166, -4.19973},
				{-4.72415, 2.76108,  -1.30315}
		};
		double b[3] = {-3.02974, 2.1059, 1.67481};
		double M[3][4] = {
				{-2.86854, 0.712385, -2.0962,  -3.02974},
				{-1.70362, -4.42166, -4.19973, 2.1059},
				{-4.72415, 2.76108,  -1.30315, 1.67481}
		};
		double x[3];

		int N = 50000000;
		double t[3] = {0, 0, 0};

		clock_t since = clock();
		for (int i = 0; i < N; ++i) {
			Solve(A, b, x);
		}
		t[0] = (clock() - since) * 1.0 / CLOCKS_PER_SEC;

		since = clock();
		for (int i = 0; i < N; ++i) {
			SolveInPlace(M);
		}
		t[1] = (clock() - since) * 1.0 / CLOCKS_PER_SEC;

		since = clock();
		for (int i = 0; i < N; ++i) {
			Solve3D(A, b, x);
		}
		t[2] = (clock() - since) * 1.0 / CLOCKS_PER_SEC;

		for (auto &i : t) {
			cout << (i /= N) << "sec\n";
		}
	}

	void coordinateConvert()
	{
		auto d = Dir(1, 0, 0);
		auto ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.x == 0 && ea.y == 0 && ea.z == M_PI_2);

		d = Dir(1, 1, 0);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.x == M_PI_4 && ea.y == 0 && ea.z == M_PI_2);

		d = Dir(0, 0, 5);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == 0);

		d = Dir(0, 0, -5);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == M_PI);
	}

	void onSurface()
	{
		auto t = Triangle(Arr<Pos, 3>{{
											  {10, 0, 0},
											  {0, 10, 0},
											  {-10, 0, 0}
									  }}, Pos(0, 0, 10), Color());
		assert(!t.hasSurfacePoint(Pos(0, 12, 10)));
		assert(t.hasSurfacePoint(Pos(0, 0, 10)));
		assert(!t.hasSurfacePoint(Pos(1, 1, 11)));
	}

	void intersect()
	{
		double t0 = 0, t1 = 0, t2 = 0, t3 = 0, tr = 0, _;
		long long N = 30000000;
		auto cube = Cube(Arr<Dir, 3>{Dir::X_AXIS, Dir::Y_AXIS, Dir::Z_AXIS},
						 Arr2D<Pos, 3, 2>{{
													{{
															 {0, 0, 0}, {30, 0, 0}
													 }},
													{{
															 {0, 0, 0}, {0, 30, 0}
													 }},
													{{
															 {0, 0, 0}, {0, 0, 50}
													 }}
											}},
		Pos(10, 30, 20), ElAg(0, 10 * DEG, 0));
		auto triangle = Triangle({Pos(0, 0, 80), {80, 0, 0}, {0, 80, 0}}, Pos(50, 0, 50), ElAg::NONROT);
		auto sphere = Sphere(600, Pos(50, 681.6 - .27, 81.6), ElAg());
		auto plane = InfPlane(Dir(0, -1, 0), Pos(0, 81.6, 0));

		clock_t since = clock();
		for (long long i = 0; i < N; ++i) {
			Ray(Pos::random(0, 100), Dir::random());
		}
		tr = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		printf("t_ref: %19.10f sec\n", tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			cube.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t0 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		printf("cube: %20.10f sec\n", t0 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			triangle.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t1 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		printf("triangle: %16.10f sec\n", t1 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			sphere.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t2 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		printf("sphere: %18.10f sec\n", t2 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			plane.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t3 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		printf("plane: %19.10f sec\n", t3 - tr);

		fflush(stdout);
	}

	void matrix()
	{
		Mat<double> a(
				Arr2D<double, 3, 3>
						{{
								 {{1, 2, 3}},
								 {{0, 1, 0}},
								 {{-1, 0, 2}}
						 }});
		a.report();
		auto b = a;
		b *= b;
		b.report();
		(b * Vec<>(1, 2, 3)).report(true);
		printf("\n\n");

		auto e = a + b;
		e.report();

		TransMat A(ElAg(0, M_PI_2, 0)), B(Pos(0, 1, 1));
		A.report();
		B.report();
		(B * A).report();
		printf("\n");
		auto c = ((B * A) * Pos(0, 1, 0));
		c.report(true);
		assert(c == Pos(0, 1, 2));
	}

	Object *ball;

	void motion()
	{
		ball->geo->translate({-1, 1.0, -0.3});
	}

	void render(int argc, char *argv[])
	{
		// ********************************************* parse cmd args *********************************************
		if (argc < 2) TERMINATE("usage: Pharosa <json_config_path>");
//		size_t n_epoch = (argc >= 2) ? (size_t) atoi(argv[1]) : 10;
//		String out_path = (argc >= 3) ? argv[2] : "test image.ppm";

		// ********************************************* define scene *********************************************
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
		ObjectList box_objs{
				new Object(Sphere(3, Pos(0, 10, 0)), Color(1.0, 0.2, 0.0), Emission(), Object::REFR),
				new Object(Sphere(5, Pos(0, 0, 0)), Color(0.5, 0.2, 1.0), Emission(), Object::DIFF),
				new Object(Sphere(3, Pos(10, 0, 0)), Color(0.5, 0.8, 1.0), Emission(), Object::SPEC),
				new Object(Sphere(3, Pos(-10, 0, 0)), Color(0.2, 0.9, 0.0), Emission(), Object::DIFF),
				new Object(Sphere(3, Pos(0, -10, 0)), Color(0.0, 0.9, 0.2), Emission(), Object::DIFF),
		};
		auto box = new BoundingSphere(Sphere(13, Pos(0, 0, 0)), box_objs);
//	auto box = new BoundingCube(box_objs);
//	auto box = new BoundingCube();
//	box->fromObjFile("res/dragon0_1 - done.obj", 40, Color(178, 255, 110) / 255.);
		box->translate({10, 10, 10});
		warn("box size: " << box->objects.size() << "\n");

		// ********************************************* init render engine *********************************************

		Renderer renderer(argv[1]); 	// load from json
		renderer.scene->fromList(singletons);
		renderer.scene->append(box);

//		renderer.setupCamera(Pos(0, 0, 280), ElAg(0, 176 * DEG, 0), 1024, 768);
//	renderer.camera().rotate(ElAg(0, -5 * DEG, 0));
//	renderer.camera().translate({0, 0, -70});

		// ********************************************* start rendering *********************************************
		renderer.start();
//	renderer.startKinetic(5, motion, n_epoch, 0, "out/kinetic");

		// ********************************************* save results *********************************************
		renderer.save();
	}
}
