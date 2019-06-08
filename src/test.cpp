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
#include "alg/KDGrid.h"

#include <set>
#include <algorithm>

using Funcs::randf;
using namespace std;

#define assertApproxEqual(x, y) assert(fabsf((x) - (y)) < EPS)

namespace Test
{

	void linear()
	{
		using namespace std;
		using namespace Linear;

		real A[3][3] = {
				{-2.86854f, 0.712385f, -2.0962f},
				{-1.70362f, -4.42166f, -4.19973f},
				{-4.72415f, 2.76108f,  -1.30315f}
		};
		real b[3] = {-3.02974f, 2.1059f, 1.67481f};
		real M[3][4] = {
				{-2.86854f, 0.712385f, -2.0962f,  -3.02974f},
				{-1.70362f, -4.42166f, -4.19973f, 2.1059f},
				{-4.72415f, 2.76108f,  -1.30315f, 1.67481f}
		};
		real x[3];

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
		assert(ea.x == 0 && ea.y == 0 && ea.z == M_PI_2F);

		d = Dir(1, 1, 0);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.x == M_PI_4F && ea.y == 0 && ea.z == M_PI_2F);

		d = Dir(0, 0, 5);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == 0);

		d = Dir(0, 0, -5);
		ea = d.getEulerAngles();
		printf("(%.2f, %.2f, %.2f)\n", ea.x / DEG, ea.y / DEG, ea.z / DEG);
		assert(ea.y == 0 && ea.z == M_PIF);
	}

	void matrix()
	{
		Mat<real> a(
				Arr2D<real, 3, 3>
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

		TransMat A(ElAg(0, M_PI_2F, 0)), B(Pos(0, 1, 1));
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
		assert(f(0.0f) == 1);
		assert(f(1.0f) == 6);
		assert(f(-2.0f) == 9);
		f -= Polynomial({-1.0f, -1, 0, 0, 2});
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
		assert(h.derivative(2.0) == 10 * powf(-2, 10));
		assert(Polynomial().derivative(1.0) == 0);
		assert(Polynomial(2, 0).derivative(1.0) == 0);
		assert(Polynomial(1, -2).derivative(10) == -2);
		assert(Polynomial({1, 2, 3}).derivative(1) == 8);
	}

	void Newton2D()
	{
		struct Fun0// : NonLinear::BinFun
		{
			real operator()(real x0, real x1) const
			{
				return 2.f * x0 - 0.5f * x1;
			}

			real d0(real x0, real x1) const
			{
				return 2.f;
			}

			real d1(real x0, real x1) const
			{
				return -0.5f;
			}
		};
		struct Fun1// : NonLinear::BinFun
		{
			Polynomial f;

			Fun1() : f({0, 0, 0, 92.16f, -460.8f, 829.44f, -645.12f, 184.32f})
			{}

			real operator()(real x0, real x1) const
			{
				return 23.04f * powf(1.f - 1.f * x0, 4) * powf(x0, 4) - powf(-2 + x1, 2);
			}

			real d0(real x0, real x1) const
			{
				return f(x0);
			}

			real d1(real x0, real x1) const
			{
				return -2 * (x1 - 2);
			}
		};
		real x0, x1;
		real since = clock();
		for (int i = 0; i < 2000000; ++i) {
			NonLinear::Solve2DTol(Fun0(), Fun1(), x0 = randf(), x1 = 0, 0.0001);
			assert(fabsf(x0 - 0.428072f) < 0.001f && fabsf(x1 - 1.71229f) < 0.001f);
		}
		printf("solve with tol in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);
		since = clock();
		for (int i = 0; i < 2000000; ++i) {
			NonLinear::Solve2DEps(Fun0(), Fun1(), x0 = randf(), x1 = 0, 0.0001);
			assert(fabsf(x0 - 0.428072f) < 0.001f && fabsf(x1 - 1.71229f) < 0.001f);
		}
		printf("solve with eps in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);

		// test singular case
		struct Fun2 : NonLinear::BinFun
		{
			real operator()(real x0, real x1) const override
			{
				return x0 * x0 + x1 * x1 - 1;
			}

			real d0(real x0, real x1) const override
			{
				return 2 * x0;
			}

			real d1(real x0, real x1) const override
			{
				return 2 * x1;
			}
		};
		struct Fun3 : NonLinear::BinFun
		{
			const real c;

			Fun3(const real c_) : c(c_)
			{}

			real operator()(real x0, real x1) const override
			{
				return x0 - x1 + c;
			}

			real d0(real x0, real x1) const override
			{
				return 1;
			}

			real d1(real x0, real x1) const override
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
		assert(fabsf(x0 - 0.707) < 0.001 && fabsf(x1 - 0.707) < 0.001);
		solvable = NonLinear::Solve2DTol(Fun2(), Fun3(0), x0 = randf(-100000, -500), x1 = randf(-100000, -500));
		assert(solvable);
		assert(fabsf(x0 + 0.707) < 0.001 && fabsf(x1 + 0.707) < 0.001);
	}

	void Newton()
	{
		struct Fun0 //: NonLinear::MonoFun    // solvable
		{
			real operator()(real x) const    // solution: x == 2 || x == 4 || x == -0.7667
			{
				return powf(2, x) - x * x;
			}

			real d(real x) const
			{
				return powf(2, x) * logf(2) - 2 * x;
			}
		};
		struct Fun1 //: NonLinear::MonoFun    // unsolvable
		{
			real operator()(real x) const
			{
				return powf(2, x) - x;
			}

			real d(real x) const
			{
				return powf(2, x) * logf(2) - 1;
			}
		};

		real x;
		bool solvable = NonLinear::SolveTol(Fun0(), x = 0.1);
		assert(solvable);
		assertApproxEqual(x, -0.7667f);
		solvable = NonLinear::SolveEps(Fun0(), x = 0.1, 1e-5);
		assert(solvable);
		assertApproxEqual(x, -0.7667f);

		solvable = NonLinear::SolveTol(Fun0(), x = 2.1);
		assert(solvable);
		assertApproxEqual(x, 2.0f);
		solvable = NonLinear::SolveEps(Fun0(), x = 2.1, 1e-5);
		assert(solvable);
		assertApproxEqual(x, 2.0f);

		solvable = NonLinear::SolveTol(Fun0(), x = 1e3);
		assert(solvable);
		assertApproxEqual(x, 4.0f);
		solvable = NonLinear::SolveEps(Fun0(), x = 1e3, 1e-5);
		assert(solvable);
		assertApproxEqual(x, 4.0f);

		solvable = NonLinear::SolveTol(Fun1(), x = randf());
		assert(!solvable);
		solvable = NonLinear::SolveEps(Fun1(), x = randf());
		assert(!solvable);

		real since = clock();
		for (int i = 0; i < 10000000; ++i) {
			solvable = NonLinear::SolveEps(Fun0(), x = randf(1.5, 2.5), 1e-4);
			assert(solvable);
			assertApproxEqual(x, 2.0f);
		}
		printf("solve with eps in %.4f sec\n", (clock() - since) / CLOCKS_PER_SEC);

		since = clock();
		for (int i = 0; i < 10000000; ++i) {
			solvable = NonLinear::SolveTol(Fun0(), x = randf(1.5, 2.5), 1e-2);
			assert(solvable);
			assertApproxEqual(x, 2.0f);
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

			inline real operator()(real x0, real x1) const
			{
				return F(x0) - G(x1);
			}

			inline real d0(real x0, real x1) const
			{
				return F.derivative(x0);
			}

			inline real d1(real x0, real x1) const
			{
				return -G.derivative(x1);
			}
		};

		struct MFun //: NonLinear::MonoFun
		{
			const Polynomial &F;
			const real b;

			MFun(const Polynomial &F_, real b_) : F(F_), b(b_)
			{}

			inline real operator()(real x) const
			{
				return F(x) - b;
			}

			inline real d(real x) const
			{
				return F.derivative(x);
			}
		};
		PolyRev surface(Polynomial(-50, 100), Polynomial(100, 0));
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
		real u, t;
		bool solved = NonLinear::Solve2DEps(f0, f1, u = 0.5, t = 1.2, 1e-2); //&& 0 <= u && u <= 1 && t > EPS;
		debug("\nu = %.4f.  t = %.4f\n", u, t);
		(ray.posAfter(t).report(true));
		assert(solved);
	}

	void kdgrid()
	{
		List<Pos> poses(1024 * 768);
		for (auto &pos:poses) {
			pos = Pos(randf(-100, 100), randf(-50, 80), randf(0, 800));
		};
		VPPtrList vps(poses.size());
		for (size_t i = 0; i < vps.size(); ++i) {
			vps[i] = new VisiblePoint;
			vps[i]->pos = poses.at(i);
			vps[i]->beta = {1, 1, 1};
			vps[i]->r = randf(0.5, 5);
		}
		KDGrid *kd_root;
		kd_root = new KDGrid(vps);
		debug("\n\033[34m[ kdgrid max_depth = %ld ]\033[0m\n", __kdgrid_max_depth__);
		VPPtrList vps_out;
		Pos pos = {0, 0, 0};
		kd_root->query(pos, 10, vps_out);
		set<VisiblePoint *> s_out, s_ans;
		shuffle(vps.begin(), vps.end(), Funcs::generator);
		for (auto vp : vps) {
			if (!vp->beta.isBlack() && (vp->pos - pos).norm() <= vp->r) {
				s_ans.insert(vp);
//				vp->pos.report(false);
//				cout << " d = " << (vp->pos - pos).norm() << " r = " << vp->r << endl;
			}
		}
		for (auto vp : vps_out) {
			s_out.insert(vp);
//			vp->pos.report(false);
//			cout << " d = " << (vp->pos - pos).norm() << " r = " << vp->r << endl;
		}
		cout << "expected: " << s_ans.size() << " vps" << endl;
		cout << "output:   " << s_out.size() << " vps" << endl;
		assert(s_out == s_ans);
		delete kd_root;
	}

	void main()
	{
		real since = clock();
//		linear();
//		coordinateConvert();
//		matrix();
//		polynomial();
//		Newton();
//		Newton2D();
//		intersectRev();
		kdgrid();
		printf("\n\033[32m[ Passed test in %.4f sec ]\033[0m\n", (clock() - since) / CLOCKS_PER_SEC);
	}
}

#undef assertApproxEqual
