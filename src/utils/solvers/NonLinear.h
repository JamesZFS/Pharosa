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
		virtual double operator()(double x0, double x1) const = 0;

		virtual double d0(double x0, double x1) const = 0;

		virtual double d1(double x0, double x1) const = 0;
	};

	struct MonoFun    // mono variable function
	{
		virtual double operator()(double x) const = 0;

		virtual double d(double x) const = 0;
	};

	// x0, x1 both init values and output, 2-var function should implement (), d00(), d01, d10(), d11()
	template<class Fun0, class Fun1>
	// criterion: change of x
	bool Solve2DTol(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double tol = 1e-3);

	template<class Fun0, class Fun1>
	// criterion: inf norm loss
	bool Solve2DEps(const Fun0 &f0, const Fun1 &f1, double &x0, double &x1, double eps = 1e-3);

	// solve one variable eqa
	template<class Fun>
	bool SolveTol(const Fun &f, double &x, double tol = 1e-3);

	template<class Fun>
	bool SolveEps(const Fun &f, double &x, double eps = 1e-3);
}

#include "NonLinear.tcc"

#endif //PHAROSA_NONLINEAR_H
