//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/solvers/Linear.h"

Triangle::Triangle(const Pos &A, const Pos &B, const Pos &C) : p{A, B, C}
{
	(n = (p[1] - p[0]) ^ (p[2] - p[0])).unitize();
}

void Triangle::applyTransform(TransMat mat)
{
	p[0] = mat * p[0];
	p[1] = mat * p[1];
	p[2] = mat * p[2];
	(n = (p[1] - p[0]) ^ (p[2] - p[0])).unitize();
}


bool Triangle::intersect(const Ray &ray, double &t) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	/*double __A[3][3] = {
			{ray.dir.x, pos[0].x - pos[1].x, pos[0].x - pos[2].x},
			{ray.dir.y, pos[0].y - pos[1].y, pos[0].y - pos[2].y},
			{ray.dir.z, pos[0].z - pos[1].z, pos[0].z - pos[2].z},
	};
	double __b[3] =
			{pos[0].x - ray.org.x,
			 pos[0].y - ray.org.y,
			 pos[0].z - ray.org.z},
			__beta, __gamma;	// cache for computing intersection*/
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

bool Triangle::hasSurfacePoint(const Pos &x) const
{
	double A[3][3], k[3], b[3];
	p[0].putToArray(&A[0][0], 3);
	p[1].putToArray(&A[0][1], 3);
	p[2].putToArray(&A[0][2], 3);
	x.putToArray(b);

	if (Linear::Solve3D(A, b, k)) {
		return (k[0] >= 0 && k[1] >= 0 && k[1] >= 0 && fabs(k[0] + k[1] + k[2] - 1) < EPS);
	}
	return false;
//	return (fabs(x % ((pos[0] - pos[1]) ^ (pos[0] - pos[2]))) < EPS);	// three points in a plane, todo some bugs
}
