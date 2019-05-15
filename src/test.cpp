//
// Created by James on 2019/4/9.
//
#include "test.h"
#include "Renderer.h"
#include "scene/Scene.h"
#include "geometric/All.h"
#include "utils/solvers/Linear.h"
#include "core/Polynomial.h"
#include "utils/solvers/NonLinear.h"

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

	void polynomial()
	{
		Polynomial f({1, 2, 3});
		f.report();
		assert(f(0.0) == 1);
		assert(f(1.0) == 6);
		assert(f(-2.0) == 9);
		f -= Polynomial({-1.0, -1, 0, 0, 2});
		assert(f.order() == 4);
		f.report();
		(f + Polynomial({0, 1})).report();
		assert(f[-1] == 0);
		assert(f[1] == 3);
		message("Poly Mul:");
		Polynomial g({2, 0, 0});
		f *= g;
		f.report();
		f.set(0, 0);
		g.set(2, 1);
		assert(f[0] == 0);
		(f * g).report();
		f = Polynomial({1, 1});
		(g = f * f * f * f).report();
		assert(g.order() == 4);
		assert(g[0] == 1 && g[1] == 4 && g[2] == 6 && g[3] == 4 && g[4] == 1);
		(g * 2).report();
		assert((g * 2.)[0] == 2);
		g *= 3;
		assert(g[3] == 12);
		message("Binomial:");
		auto h = Polynomial::binomial(2, -2, 10);
		h.report();
		assert(h.derivative(2.0) == 10 * pow(-2, 10));
		assert(Polynomial().derivative(1.0) == 0);
		assert(Polynomial({2, 0}).derivative(1.0) == 0);
		assert(Polynomial({1, -2}).derivative(10) == -2);
		assert(Polynomial({1, 2, 3}).derivative(1) == 8);
	}

	void Newton()
	{
		struct Fun0 : NonLinear::BinFun
		{
			double operator()(double x0, double x1) const override
			{
				return 2. * x0 - 0.5 * x1;
			}

			double d0(double x0, double x1) const override
			{
				return 2.;
			}

			double d1(double x0, double x1) const override
			{
				return -0.5;
			}
		};
		struct Fun1 : NonLinear::BinFun
		{
			Polynomial f;

			Fun1() : f({0, 0, 0, 92.16, -460.8, 829.44, -645.12, 184.32})
			{}

			double operator()(double x0, double x1) const override
			{
				return 23.04 * pow(1. - 1. * x0, 4) * pow(x0, 4) - pow(-2 + x1, 2);
			}

			double d0(double x0, double x1) const override
			{
				return f(x0);
			}

			double d1(double x0, double x1) const override
			{
				return -2 * (x1 - 2);
			}
		};
		for (int i = 0; i < 3000000; ++i) {
			double x0 = 0.1, x1 = 0.1;
			NonLinear::Solve2D(Fun0(), Fun1(), x0, x1, 0.0001);
//		printf("x0 = %.4f\nx1 = %.4f", x0, x1);
			assert(fabs(x0 - 0.428072) < 0.001 && fabs(x1 - 1.71229) < 0.001);
		}
	}

	void main()
	{
		double since = clock();
//		linear();
//		coordinateConvert();
//		matrix();
//		polynomial();
		Newton();
		printf("\n\033[32m[ Passed test in %.4f sec ]\033[0m\n", (clock() - since) / CLOCKS_PER_SEC);
	}
}
