//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_LINEAR_H
#define PHAROSA_LINEAR_H

#include "../../Pharosa.h"

namespace Linear
{
	// Solve2D A x == b (n_dim == n, A: matrix)
	template<int n>
	bool Solve(double (&A)[n][n], double (&b)[n], double (&x)[n]);

	// Solve2D A x == b (n_dim == n, A: matrix) for debuging use
	template<int n>
	bool SolveDebug(double (&A)[n][n], double (&b)[n], double (&x)[n]);

	// M: expanded Matrix (n x n+1)
	template<int n>
	bool SolveInPlace(double (&M)[n][n + 1]);

	// solve 3 dim linear eqls, fastest version
	bool Solve3D(double (&A)[3][3], double (&b)[3], double (&x)[3]);

	bool Solve3D(double (&A)[3][3], double (&b)[3], double &x0, double &x1, double &x2);

	// solve A x == b
	inline bool Solve2D(double A00, double A01, double A10, double A11, double b0, double b1, double &x0, double &x1);
}

#include "Linear.tcc"

#endif //PHAROSA_LINEAR_H
