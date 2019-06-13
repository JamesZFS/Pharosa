//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/solvers/Linear.h"

Triangle::Triangle(const Pos &A, const Pos &B, const Pos &C) :
		p{A, B, C}, n((B - A) ^ (C - A)), surface_area(0.5f * n.norm())
{
	if (n.sqr() == 0) throw DegenerateCase();
	n.unitize();
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}

void Triangle::applyTransform(const TransMat &mat)
{
	p[0] = mat * p[0];
	p[1] = mat * p[1];
	p[2] = mat * p[2];
	(n = (p[1] - p[0]) ^ (p[2] - p[0])).unitize();
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}

bool Triangle::intersect(const Ray &ray, real &t, Intersection &isect) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	real A[3][3], b[3], beta, gamma;
	ray.dir.putToArray(&A[0][0], 3);
	(p[0] - p[1]).putToArray(&A[0][1], 3);
	(p[0] - p[2]).putToArray(&A[0][2], 3);
	(p[0] - ray.org).putToArray(b);

	real ti;
	if (!Linear::Solve3D(A, b, ti, beta, gamma)) return false;    // cannot solve or root is not unique
	// solvable and has only root
	if (ti < EPS || beta < 0 || 1 < beta || gamma < 0 || 1 < gamma || 1 < beta + gamma || ti >= t) return false;
	t = ti;        // update
	return true;
}

void Triangle::report() const
{
	printf("<Triangle> vertices: ");
	p[0].report();
	p[1].report();
	p[2].report();
	printf(" normal: ");
	n.report(true);
}

const char *Triangle::DegenerateCase::what() const noexcept
{
	return "this triangle is generated, given A B C are in a line.";
}
