//
// Created by James on 2019/5/15.
//

#include "Linear.h"

namespace NonLinear
{
	template<class Fun0, class Fun1>
	bool Solve2DTol(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double tol)
	{
		double s0, s1, x0_prev = x0, x1_prev = x1;
		while (true) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1))
				return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabs(x0 - x0_prev) < tol && fabs(x1 - x1_prev) < tol) break;    // inf norm
			x0_prev = x0, x1_prev = x1;
		}
		return true;
	}

	template<class Fun0, class Fun1>
	bool Solve2DEps(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double eps)
	{
		double s0, s1;
		while (true) {    // Newton iteration
			// Jacobi
			if (!Linear::Solve2D(f0.d0(x0, x1), f0.d1(x0, x1), f1.d0(x0, x1), f1.d1(x0, x1),
								 f0(x0, x1), f1(x0, x1), s0, s1))
				return false;    // singular
			x0 -= s0, x1 -= s1;    // step
			if (fabs(f0(x0, x1)) < eps && fabs(f1(x0, x1)) < eps) break;    // inf norm
		}
		return true;
	}
}
