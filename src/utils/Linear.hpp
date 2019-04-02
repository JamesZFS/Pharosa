//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_LINEAR_HPP
#define RAYTRACKER_LINEAR_HPP

// Linear algebra solver

#include "../lib.h"

// Solve A b == x (n_dim == n, A: matrix)
bool SolveLinear(double *A, double *b, double *x, int n)
{
	double M[n][n + 1];		// expand to (n+1) cols
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			M[i][j] = (A + i * n)[j];
		}
		M[i][n] = b[i];
	}
	debug("before reduction:\n");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n + 1; ++j) {
			debug("%.2f ", M[i][j]);
		}
		debug("\n");
	}
	debug("\n");

	for (int k = 0; k < n; ++k) {    // n principle component
		double col_max = fabs(M[k][k]), fm;
		int maxi = k;
		for (int i = k + 1; i < n; ++i) {    // find the max in k-th col
			if ((fm = fabs(M[i][k])) > col_max) {
				col_max = fm;
				maxi = i;
			}
		}
		if (col_max < EPS) {        // singular matrix
			return false;        // solving failed
		}
		// swap k-th and maxi-th row
		for (int j = k; j < n + 1; ++j) {
			std::swap(M[k][j], M[maxi][j]);
		}

		// perform reduction
		for (int i = k + 1; i < n; ++i) {
			double r = -M[i][k] / M[k][k];
			M[i][k] = 0;	//todo
			for (int j = k + 1; j < n + 1; ++j) {
				M[i][j] += r * M[k][j];
			}
		}
	}
	debug("after reduction:\n");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n + 1; ++j) {
			debug("%.2f ", M[i][j]);
		}
		debug("\n");
	}
	debug("\n");

	// regressive solving
	for (int k = n - 1; k >= 0; --k) {
		for (int i = 0; i < k; ++i) {
			M[i][n] += -M[i][k] / M[k][k] * M[k][n];
			M[i][k] = 0;    //todo
		}
		M[k][n] /= M[k][k]; // todo
		M[k][k] = 1;	//todo
		x[k] = M[k][n];
	}

	debug("after solving:\n");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n + 1; ++j) {
			debug("%.2f ", M[i][j]);
		}
		debug("\n");
	}
	debug("\n");

	return true;    // solving success
}
/*bool SolveLinear(double **A, double *b, double *x, unsigned int n)
{
	auto **M = new double *[n];
	for (int i = 0; i < n; ++i) {
		M[i] = new double[n + 1];    // expand to (n+1) cols
		for (int j = 0; j < n; ++j) {
			M[i][j] = A[i][j];
		}
		M[i][n] = b[i];
	}
	for (int k = 0; k < n; ++k) {    // n principle component
		double col_max = fabs(M[k][k]), fm;
		int max_row_index = k;
		for (int i = k + 1; i < n; ++i) {    // find the max in k-th col
			if ((fm = fabs(M[i][k])) > col_max) {
				col_max = fm;
				max_row_index = i;
			}
		}
		if (col_max < EPS) {        // singular matrix
			for (int i = 0; i < n; ++i) {
				delete[] M[i];
			}
			delete[] M;
			return false;        // solving failed
		}
		double temp;
		// swap k-th and m-th row
		for (int m = 0; m < n + 1; ++m) {
			temp = M[k][m];
			M[k][m] = M[max_row_index][m];
			M[max_row_index][m] = temp;
		}

		// do reduction
		for (int i = k + 1; i < n; ++i) {
			for (int j = k + 1; j < n + 1; ++j) {
				M[i][j] = M[k][k] * M[i][j] / M[i][k] - M[k][j];
			}
		}
	}
	// regressive solving
	for (int i = n - 1; i >= 0; --i) {
		x[i] = M[i][n];
		for (int j = i + 1; j < n; ++j) {
			x[i] -= M[i][j] * x[j];
		}
		x[i] /= M[i][i];
	}
	for (int i = 0; i < n; ++i) {
		delete[] M[i];
	}
	delete[] M;
	return true;    // solving success
}*/

#endif //RAYTRACKER_LINEAR_HPP
