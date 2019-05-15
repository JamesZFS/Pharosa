//
// Created by James on 2019/5/15.
//

#include "NonLinear.h"
#include "Linear.h"

namespace NonLinear
{
	bool Solve2D(const BinFun &f0, const BinFun &f1, double &x0, double &x1, double tol)
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
}
