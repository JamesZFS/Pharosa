//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_LINEAR_H
#define PHAROSA_LINEAR_H

#include "../../Pharosa.h"

namespace Linear
{
	// Solve2DTol A x == b (n_dim == n, A: matrix)
	template<int n>
	bool Solve(real (&A)[n][n], real (&b)[n], real (&x)[n]);

	// Solve2DTol A x == b (n_dim == n, A: matrix) for debuging use
	template<int n>
	bool SolveDebug(real (&A)[n][n], real (&b)[n], real (&x)[n]);

	// M: expanded Matrix (n x n+1)
	template<int n>
	bool SolveInPlace(real (&M)[n][n + 1]);

	// solve 3 dim linear eqls, fastest version
	bool Solve3D(real (&A)[3][3], real (&b)[3], real (&x)[3]);

	bool Solve3D(real (&A)[3][3], real (&b)[3], real &x0, real &x1, real &x2);

	// solve A x == b
	inline bool Solve2D(real A00, real A01, real A10, real A11, real b0, real b1, real &x0, real &x1);
}

#include "Linear.tcc"

#endif //PHAROSA_LINEAR_H
