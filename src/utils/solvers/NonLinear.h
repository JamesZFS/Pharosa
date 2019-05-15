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

	// x0, x1 both init values and output, 2-var function should implement (), d00(), d01, d10(), d11()
	bool Solve2D(const BinFun &f0, const BinFun &f1, double &x0, double &x1, double tol = 1e-3);
}

#endif //PHAROSA_NONLINEAR_H
