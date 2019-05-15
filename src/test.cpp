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

using Funcs::randf;

#define assertApproxEqual(x, y) assert(fabs((x) - (y)) < EPS)

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
		(f + Polynomial(0, 1)).report();
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
		f = Polynomial(1, 1);
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
		assert(Polynomial(2, 0).derivative(1.0) == 0);
		assert(Polynomial(1, -2).derivative(10) == -2);
		assert(Polynomial({1, 2, 3}).derivative(1) == 8);
	}

	void Newton2D()
	{
		struct Fun0// : NonLinear::BinFun
		{
			double operator()(double x0, double x1) const
			{
				return 2. * x0 - 0.5 * x1;
			}

			double d0(double x0, double x1) const
			{
				return 2.;
			}

			double d1(double x0, double x1) const
			{
				return -0.5;
			}
		};
		struct Fun1// : NonLinear::BinFun
		{
			Polynomial f;

			Fun1() : f({0, 0, 0, 92.16, -460.8, 829.44, -645.12, 184.32})
			{}

			double operator()(double x0, double x1) const
			{
				return 23.04 * pow(1. - 1. * x0, 4) * pow(x0, 4) - pow(-2 + x1, 2);
			}

			double d0(double x0, double x1) const
			{
				return f(x0);
			}

			double d1(double x0, double x1) const
			{
				return -2 * (x1 - 2);
			}
		};
		double x0, x1;
		double since = clock();
		for (int i = 0; i < 2000000; ++i) {
			NonLinear::Solve2DTol(Fun0(), Fun1(), x0 = randf(), x1 = 0, 0.0001);
			assert(fabs(x0 - 0.428072) < 0.001 && fabs(x1 - 1.71229) < 0.001);
		}
		printf("solve with tol in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);
		since = clock();
		for (int i = 0; i < 2000000; ++i) {
			NonLinear::Solve2DEps(Fun0(), Fun1(), x0 = randf(), x1 = 0, 0.0001);
			assert(fabs(x0 - 0.428072) < 0.001 && fabs(x1 - 1.71229) < 0.001);
		}
		printf("solve with eps in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);

		// test singular case
		struct Fun2 : NonLinear::BinFun
		{
			double operator()(double x0, double x1) const override
			{
				return x0 * x0 + x1 * x1 - 1;
			}

			double d0(double x0, double x1) const override
			{
				return 2 * x0;
			}

			double d1(double x0, double x1) const override
			{
				return 2 * x1;
			}
		};
		struct Fun3 : NonLinear::BinFun
		{
			const double c;

			Fun3(const double c_) : c(c_)
			{}

			double operator()(double x0, double x1) const override
			{
				return x0 - x1 + c;
			}

			double d0(double x0, double x1) const override
			{
				return 1;
			}

			double d1(double x0, double x1) const override
			{
				return -1;
			}
		};
		bool solvable = NonLinear::Solve2DEps(Fun2(), Fun3(2), x0 = randf(), x1 = randf());
		assert(!solvable);
		solvable = NonLinear::Solve2DTol(Fun2(), Fun3(-1.8), x0 = randf(), x1 = randf());
		assert(!solvable);
		solvable = NonLinear::Solve2DEps(Fun2(), Fun3(0), x0 = randf(), x1 = randf());
		assert(solvable);
		assert(fabs(x0 - 0.707) < 0.001 && fabs(x1 - 0.707) < 0.001);
		solvable = NonLinear::Solve2DTol(Fun2(), Fun3(0), x0 = randf(-100000, -500), x1 = randf(-100000, -500));
		assert(solvable);
		assert(fabs(x0 + 0.707) < 0.001 && fabs(x1 + 0.707) < 0.001);
	}

	void Newton()
	{
		struct Fun0 //: NonLinear::MonoFun    // solvable
		{
			double operator()(double x) const    // solution: x == 2 || x == 4 || x == -0.7667
			{
				return pow(2, x) - x * x;
			}

			double d(double x) const
			{
				return pow(2, x) * log(2) - 2 * x;
			}
		};
		struct Fun1 //: NonLinear::MonoFun    // unsolvable
		{
			double operator()(double x) const
			{
				return pow(2, x) - x;
			}

			double d(double x) const
			{
				return pow(2, x) * log(2) - 1;
			}
		};

		double x;
		bool solvable = NonLinear::SolveTol(Fun0(), x = 0.1);
		assert(solvable);
		assertApproxEqual(x, -0.7667);
		solvable = NonLinear::SolveEps(Fun0(), x = 0.1, 1e-5);
		assert(solvable);
		assertApproxEqual(x, -0.7667);

		solvable = NonLinear::SolveTol(Fun0(), x = 2.1);
		assert(solvable);
		assertApproxEqual(x, 2.0);
		solvable = NonLinear::SolveEps(Fun0(), x = 2.1, 1e-5);
		assert(solvable);
		assertApproxEqual(x, 2.0);

		solvable = NonLinear::SolveTol(Fun0(), x = 1e3);
		assert(solvable);
		assertApproxEqual(x, 4.0);
		solvable = NonLinear::SolveEps(Fun0(), x = 1e3, 1e-5);
		assert(solvable);
		assertApproxEqual(x, 4.0);

		solvable = NonLinear::SolveTol(Fun1(), x = randf());
		assert(!solvable);
		solvable = NonLinear::SolveEps(Fun1(), x = randf());
		assert(!solvable);

		double since = clock();
		for (int i = 0; i < 10000000; ++i) {
			solvable = NonLinear::SolveEps(Fun0(), x = randf(1.5, 2.5), 1e-4);
			assert(solvable);
			assertApproxEqual(x, 2.0);
		}
		printf("solve with eps in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);

		since = clock();
		for (int i = 0; i < 10000000; ++i) {
			solvable = NonLinear::SolveTol(Fun0(), x = randf(1.5, 2.5), 1e-2);
			assert(solvable);
			assertApproxEqual(x, 2.0);
		}
		printf("solve with tol in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);
	}

	void intersectRev()
	{
		struct BFun //: NonLinear::BinFun
		{
			const Polynomial &F, &G;

			BFun(const Polynomial &F_, const Polynomial &G_) : F(F_), G(G_)    // F(x0) - G(x1)
			{}

			inline double operator()(double x0, double x1) const
			{
				return F(x0) - G(x1);
			}

			inline double d0(double x0, double x1) const
			{
				return F.derivative(x0);
			}

			inline double d1(double x0, double x1) const
			{
				return -G.derivative(x1);
			}
		};

		struct MFun //: NonLinear::MonoFun
		{
			const Polynomial &F;
			const double b;

			MFun(const Polynomial &F_, double b_) : F(F_), b(b_)
			{}

			inline double operator()(double x) const
			{
				return F(x) - b;
			}

			inline double d(double x) const
			{
				return F.derivative(x);
			}
		};
		PolyRevolution surface(Polynomial(-50, 100), Polynomial(100, 0));
		surface.phi.report();
		surface.psi.report();
		surface.psi_2.report();
		Ray ray({0, 0, 400}, {0, 50, -400});    // to local
		Polynomial L0, L1, L2;
		L0.set(0, ray.org.x), L0.set(1, ray.dir.x);
		L1.set(0, ray.org.y), L1.set(1, ray.dir.y);
		L2.set(0, ray.org.z), L2.set(1, ray.dir.z);
		auto P = L1 * L1 + L2 * L2;
//		debug("\nL0:\n");
//		L0.report();
//		debug("\nL1:\n");
//		L1.report();
//		debug("\nL2:\n");
//		L2.report();
		BFun f0(surface.phi, L0);
		BFun f1(surface.psi_2, P);
		assertApproxEqual(f0(0.5, 0), 0);
		assertApproxEqual(f0(0.5, 1), 0);
//		message(L1(310.087));
//		message(L2(310.087));
//		message((L1 * L1 + L2 * L2)(310.087));
//		message(f1.G(310.087));
//		message("psi^2:\n");
//		surface.psi_2.report();
//		message("test psi^2 = " << f1.F(0.5) << "\n");
//		message("psi^2 - (L1^2 + L2^2)(t) = " << f1.F(0.5) - (L1 * L1 + L2 * L2)(310.087) << "\n");
//		message(f1(0.5, 310.087));
//		assertApproxEqual(f1(0.8, 1.2), 0);
		double u, t;
		bool solved = NonLinear::Solve2DEps(f0, f1, u = 0.5, t = 1.2, 1e-2); //&& 0 <= u && u <= 1 && t > EPS;
		debug("\nu = %.4f.  t = %.4f\n", u, t);
		(ray.org + ray.dir * t).report(true);
		assert(solved);
	}

	void main()
	{
		double since = clock();
//		linear();
//		coordinateConvert();
//		matrix();
//		polynomial();
//		Newton();
//		Newton2D();
		intersectRev();
		printf("\n\033[32m[ Passed test in %.4f sec ]\033[0m\n", (clock() - since) / CLOCKS_PER_SEC);
	}
}

#undef assertApproxEqual
