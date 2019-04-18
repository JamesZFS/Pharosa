//
// Created by James on 2019/4/9.
//

#include "src/lib.h"
#include "src/utils/solvers/Linear.h"
#include "src/utils/funcs.hpp"
#include "src/core/Vec.h"
#include "src/core/Mat.h"
#include "src/geometric/All.h"

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
		auto d = Dir(0, 2, 0);
		auto ea = d.getEulerAngles();
		debug("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.x == M_PI_2 && ea.y == 0 && ea.z == M_PI_2);

		d = Dir(1, 1, 0);
		ea = d.getEulerAngles();
		debug("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.x == M_PI_4 && ea.y == 0 && ea.z == M_PI_2);

		d = Dir(0, 0, 5);
		ea = d.getEulerAngles();
		debug("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == 0);

		d = Dir(0, 0, -5);
		ea = d.getEulerAngles();
		debug("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == M_PI);
	}

	void onSurface()
	{
		auto t = Triangle(new Pos[3]{{10,  0,  0},
									 {0,   10, 0},
									 {-10, 0,  0}}, Pos(0, 0, 10), Color());
		assert(!t.hasSurfacePoint(Pos(0, 12, 10)));
		assert(t.hasSurfacePoint(Pos(0, 0, 10)));
		assert(!t.hasSurfacePoint(Pos(1, 1, 11)));
	}

	void intersect()
	{
		double t0 = 0, t1 = 0, t2 = 0, t3 = 0, tr = 0, _;
		long long N = 30000000;
		Dir d[3]{Dir::X_AXIS, Dir::Y_AXIS, Dir::Z_AXIS};
		Pos cp[3][2]{{{0, 0, 0}, {30, 0,  0}},
					 {{0, 0, 0}, {0,  30, 0}},
					 {{0, 0, 0}, {0,  0,  50}}};
		Pos tp[3]{Pos(0, 0, 80), {80, 0, 0}, {0, 80, 0}};

		auto cube = Cube(d, cp, Pos(10, 30, 20), ElAg(0, 10 * DEG, 0));
		auto triangle = Triangle(tp, Pos(50, 0, 50), ElAg::NONROT);
		auto sphere = Sphere(600, Pos(50, 681.6 - .27, 81.6), ElAg());
		auto plane = InfPlane(Dir(0, -1, 0), Pos(0, 81.6, 0));

		clock_t since = clock();
		for (long long i = 0; i < N; ++i) {
			Ray(Pos::random(0, 100), Dir::random());
		}
		tr = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		debug("t_ref: %19.10f sec\n", tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			cube.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t0 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		debug("cube: %20.10f sec\n", t0 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			triangle.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t1 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		debug("triangle: %16.10f sec\n", t1 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			sphere.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t2 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		debug("sphere: %18.10f sec\n", t2 - tr);

		since = clock();
		for (long long i = 0; i < N; ++i) {
			plane.intersect(Ray(Pos::random(0, 100), Dir::random()), _);
		}
		t3 = (clock() - since) * 1.0 / CLOCKS_PER_SEC;
		debug("plane: %19.10f sec\n", t3 - tr);

		fflush(stdout);
	}

	void matrix()
	{
//		Sphere s(1, Pos::ORIGIN, ElAg::NONROT);
//		Sphere a = s;

		double d[3][3] = {
				{1,  2, 3},
				{0,  1, 0},
				{-1, 0, 2}
		};
		Mat<double> a(d);
		a.report();
		auto b = a;
		b *= b;
		b.report();
		(b * Vec<>(1, 2, 3)).report(true);
		debug("\n\n");

		auto e = a + b;
		e.report();

		TransMat A(ElAg(0, M_PI_2, 0)), B(Pos(0, 1, 1));
		A.report();
		B.report();
		(B * A).report();
		debug("\n");
		auto c = ((B * A) * Pos(0, 1, 0));
		c.report(true);
		assert(c == Pos(0, 1, 2));
	}
}
