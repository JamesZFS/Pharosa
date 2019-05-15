//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/solvers/Linear.h"

Triangle::Triangle(const Pos &A, const Pos &B, const Pos &C) : p{A, B, C}, n((B - A) ^ (C - A))
{
	n.unitize();
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}

void Triangle::applyTransform(TransMat mat)
{
	p[0] = mat * p[0];
	p[1] = mat * p[1];
	p[2] = mat * p[2];
	(n = (p[1] - p[0]) ^ (p[2] - p[0])).unitize();
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}


bool Triangle::intersect(const Ray &ray, double &t) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	double A[3][3], b[3], beta, gamma;
	ray.dir.putToArray(&A[0][0], 3);
	(p[0] - p[1]).putToArray(&A[0][1], 3);
	(p[0] - p[2]).putToArray(&A[0][2], 3);
	(p[0] - ray.org).putToArray(b);

	if (Linear::Solve3D(A, b, t, beta, gamma)) {    // solvable and has only root
		return (t > EPS && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1 && beta + gamma <= 1);
	}
	return false;    // cannot solve or root is not unique
}
