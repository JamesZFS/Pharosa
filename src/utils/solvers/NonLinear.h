//
// Created by James on 2019/5/15.
//

#ifndef PHAROSA_NONLINEAR_H
#define PHAROSA_NONLINEAR_H

#include "../../Pharosa.h"

namespace NonLinear
{
	struct BinFun    // binary function
	{
		virtual real operator()(real x0, real x1) const = 0;

		virtual real d0(real x0, real x1) const = 0;

		virtual real d1(real x0, real x1) const = 0;
	};

	struct MonoFun    // mono variable function
	{
		virtual real operator()(real x) const = 0;

		virtual real d(real x) const = 0;
	};

	// x0, x1 both init values and output, 2-var function should implement (), d00(), d01, d10(), d11()
	template<class Fun0, class Fun1>
	// criterion: change of x
	bool Solve2DTol(const Fun0 &f0, const Fun1 &f1, real &x0, real &x1, real tol = 1e-3);

	template<class Fun0, class Fun1>
	// criterion: inf norm loss
	bool Solve2DEps(const Fun0 &f0, const Fun1 &f1, real &x0, real &x1, real eps = 1e-3);

	// solve one variable eqa
	template<class Fun>
	bool SolveTol(const Fun &f, real &x, real tol = 1e-3);

	template<class Fun>
	bool SolveEps(const Fun &f, real &x, real eps = 1e-3);
}

#include "NonLinear.tcc"

#endif //PHAROSA_NONLINEAR_H
