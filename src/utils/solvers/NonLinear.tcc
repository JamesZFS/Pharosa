//
// Created by James on 2019/5/15.
//

#include "Linear.h"

#define MAX_ITER 1000
//#define __PRINT_N_ITER__

namespace NonLinear
{
	template<class Fun0, class Fun1>
	bool Solve2DTol(const Fun0 &f0, const Fun1 &f1, real &x0, real &x1, real tol)
	{
		real s0, s1, x0_prev = x0, x1_prev = x1;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1)) return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabsf(x0 - x0_prev) < tol && fabsf(x1 - x1_prev) < tol) {
#ifdef __PRINT_N_ITER__
				safe_debug("solved, # iter = %ld\n", i);
#endif
				return true;    // inf norm
			}
			x0_prev = x0, x1_prev = x1;
		}
		return false;    // still not converged yet
	}

	template<class Fun0, class Fun1>
	bool Solve2DEps(const Fun0 &f0, const Fun1 &f1, real &x0, real &x1, real eps)
	{
		real s0, s1;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1)) return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabsf(f0(x0, x1)) < eps && fabsf(f1(x0, x1)) < eps) {
#ifdef __PRINT_N_ITER__
				safe_debug("solved, # iter = %ld\n", i);
#endif
				return true;    // inf norm
			}
		}
		return false;
	}

	template<class Fun>
	bool SolveTol(const Fun &f, real &x, real tol)
	{
		real d, x_prev = x;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			d = f.d(x);
			if (fabsf(d) < EPS) return false;
			x -= f(x) / d;
			if (fabsf(x - x_prev) < tol) {
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
	bool SolveEps(const Fun &f, real &x, real eps)
	{
		real d;
		for (size_t i = 0; i < MAX_ITER; ++i) {    // Newton iteration
			d = f.d(x);
			if (fabsf(d) < EPS) return false;
			x -= f(x) / d;
			if (fabsf(f(x)) < eps) {
#ifdef __PRINT_N_ITER__
				safe_debug("# iter = %ld\n", i);
#endif
				return true;
			}
		}
		return false;
	}
}
