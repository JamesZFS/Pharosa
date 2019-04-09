//
// Created by James on 2019/4/9.
//

#include "src/lib.h"
#include "src/utils/solvers/Linear.h"
#include "src/utils/funcs.hpp"
#include "src/core/Vec.h"
#include "src/scene/All.h"

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

	void rotate()
	{
		auto p = InfPlane(Dir(1, 0, 0), Pos(0, 0, 0), {1, 1, 1}, {1, 1, 1}, Object::DIFF);
		assert(p.ea == ElAg(0, 0, M_PI_2));
	}

	void onSurface()
	{
		auto t = Triangle(new Pos[3]{{10,  0, 0},
									 {0,  10, 0},
									 {-10, 0, 0}}, Pos(0, 0, 10), Color());
		assert(!t.hasSurfacePoint(Pos(0, 12, 10)));
		assert(t.hasSurfacePoint(Pos(0, 0, 10)));
		assert(!t.hasSurfacePoint(Pos(1, 1, 11)));
	}
}
