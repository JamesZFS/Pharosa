//
// Created by James on 2019/4/9.
//

#include "Linear.h"

// solve 3 dim linear eqls, fastest version
bool Linear::Solve3D(real (&A)[3][3], real (&b)[3], real (&x)[3])
{
	real det = -A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][2] * A[2][0] + A[0][2] * A[1][0] * A[2][1] -
				 A[0][0] * A[1][2] * A[2][1] - A[0][1] * A[1][0] * A[2][2] + A[0][0] * A[1][1] * A[2][2];
	if (fabsf(det) < EPS) return false;    // singular

	x[0] = (-b[2] * A[0][2] * A[1][1] + b[2] * A[0][1] * A[1][2] + b[1] * A[0][2] * A[2][1] -
			b[0] * A[1][2] * A[2][1] - b[1] * A[0][1] * A[2][2] + b[0] * A[1][1] * A[2][2]) / det;
	x[1] = (b[2] * A[0][2] * A[1][0] - b[2] * A[0][0] * A[1][2] - b[1] * A[0][2] * A[2][0] +
			b[0] * A[1][2] * A[2][0] + b[1] * A[0][0] * A[2][2] - b[0] * A[1][0] * A[2][2]) / det;
	x[2] = (-b[2] * A[0][1] * A[1][0] + b[2] * A[0][0] * A[1][1] + b[1] * A[0][1] * A[2][0] -
			b[0] * A[1][1] * A[2][0] - b[1] * A[0][0] * A[2][1] + b[0] * A[1][0] * A[2][1]) / det;
	return true;
}

bool Linear::Solve3D(real (&A)[3][3], real (&b)[3], real &x0, real &x1, real &x2)
{
	real det = -A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][2] * A[2][0] + A[0][2] * A[1][0] * A[2][1] -
				 A[0][0] * A[1][2] * A[2][1] - A[0][1] * A[1][0] * A[2][2] + A[0][0] * A[1][1] * A[2][2];
	if (fabsf(det) < EPS) return false;    // singular

	x0 = (-b[2] * A[0][2] * A[1][1] + b[2] * A[0][1] * A[1][2] + b[1] * A[0][2] * A[2][1] -
		  b[0] * A[1][2] * A[2][1] - b[1] * A[0][1] * A[2][2] + b[0] * A[1][1] * A[2][2]) / det;
	x1 = (b[2] * A[0][2] * A[1][0] - b[2] * A[0][0] * A[1][2] - b[1] * A[0][2] * A[2][0] +
		  b[0] * A[1][2] * A[2][0] + b[1] * A[0][0] * A[2][2] - b[0] * A[1][0] * A[2][2]) / det;
	x2 = (-b[2] * A[0][1] * A[1][0] + b[2] * A[0][0] * A[1][1] + b[1] * A[0][1] * A[2][0] -
		  b[0] * A[1][1] * A[2][0] - b[1] * A[0][0] * A[2][1] + b[0] * A[1][0] * A[2][1]) / det;
	return true;
}
