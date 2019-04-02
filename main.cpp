//
// Created by think on 2019/4/1.
//

#include "src/lib.h"
#include "src/utils/Solvers.h"
#include <iostream>

using namespace std;

int main()
{
	debug("Hello Ray Tracking!\n");


	double A[3][3] = {{4.30704,  3.15821,   -1.77939},
					  {-4.10422, -0.928472, 2.88121},
					  {-1.22797, 3.67243,   3.836}};

	double b[3] = {7.08961, 5.97111, 4.22353};
	double x[3];

	double M[3][4] = {{4.30704,  3.15821,   -1.77939, 7.08961},
					  {-4.10422, -0.928472, 2.88121,  5.97111},
					  {-1.22797, 3.67243,   3.836,    4.22353}};

	auto solvable = Solver::SolveLinear(&A[0][0], b, x, 3);
	if (solvable) {
		debug("x = (%.5f, %.5f, %.5f)\n", x[0], x[1], x[2]);
	}
	else {
		debug("singular\n");
	}

	solvable = Solver::SolveLinearInPlace(&M[0][0], 3);
	if (solvable) {
		debug("x = (%.5f, %.5f, %.5f)\n", M[0][3], M[1][3], M[2][3]);
	}
	else {
		debug("singular\n");
	}

	return 0;
}
