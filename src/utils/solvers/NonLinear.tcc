//
// Created by James on 2019/5/15.
//

#include "Linear.h"
#define MAX_ITER 1000
#define __PRINT_N_ITER__

namespace NonLinear
{
	template<class Fun0, class Fun1>
	bool Solve2DTol(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double tol)
	{
		double s0, s1, x0_prev = x0, x1_prev = x1;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1))
				return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabs(x0 - x0_prev) < tol && fabs(x1 - x1_prev) < tol) {
#ifdef __PRINT_N_ITER__
				safe_debug("# iter = %ld\n", i);
#endif
				return true;    // inf norm
			}
			x0_prev = x0, x1_prev = x1;
		}
		return false;	// still not converged yet
	}

	template<class Fun0, class Fun1>
	bool Solve2DEps(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double eps)
	{
		double s0, s1;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1))
				return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabs(f0(x0, x1)) < eps && fabs(f1(x0, x1)) < eps) {
#ifdef __PRINT_N_ITER__
				safe_debug("# iter = %ld\n", i);
#endif
				return true;    // inf norm
			}
		}
		return false;
	}

	template<class Fun>
	bool SolveTol(const Fun &f, double &x, double tol)
	{
		double d, x_prev = x;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			d = f.d(x);
			if (fabs(d) < EPS) return false;
			x -= f(x) / d;
			if (fabs(x - x_prev) < tol) {
#ifdef __PRINT_N_ITER__
				safe_debug("# iter = %ld\n", i);
#endif
				return true;
			}
			x_prev = x;
		}
		return false;
	}

	template<class Fun>
	bool SolveEps(const Fun &f, double &x, double eps)
	{
		double d;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			d = f.d(x);
			if (fabs(d) < EPS) return false;
			x -= f(x) / d;
			if (fabs(f(x)) < eps) {
#ifdef __PRINT_N_ITER__
				safe_debug("# iter = %ld\n", i);
#endif
				return true;
			}
		}
		return false;
	}
}
