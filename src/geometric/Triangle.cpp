//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/solvers/Linear.h"

Triangle::Triangle(Arr<Pos, 3> &&p_, const Pos &pos_, const ElAg &euler_angles_) :
		Geometry(pos_, euler_angles_), p(p_)
{
	applyTransform();
}

void Triangle::applyTransform()
{
	gp[0] = mat * p[0];
	gp[1] = mat * p[1];
	gp[2] = mat * p[2];
	(n = (gp[0] - gp[1]) ^ (gp[0] - gp[2])).unitize();
}


bool Triangle::intersect(const Ray &ray, double &t) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	/*double __A[3][3] = {
			{ray.dir.x, gp[0].x - gp[1].x, gp[0].x - gp[2].x},
			{ray.dir.y, gp[0].y - gp[1].y, gp[0].y - gp[2].y},
			{ray.dir.z, gp[0].z - gp[1].z, gp[0].z - gp[2].z},
	};
	double __b[3] =
			{gp[0].x - ray.org.x,
			 gp[0].y - ray.org.y,
			 gp[0].z - ray.org.z},
			__beta, __gamma;	// cache for computing intersection*/
	double A[3][3], b[3], beta, gamma;
	ray.dir.putToArray(&A[0][0], 3);
	(gp[0] - gp[1]).putToArray(&A[0][1], 3);
	(gp[0] - gp[2]).putToArray(&A[0][2], 3);
	(gp[0] - ray.org).putToArray(b);

	if (Linear::Solve3D(A, b, t, beta, gamma)) {    // solvable and has only root	todo use 3D
		return (t > EPS && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1 && beta + gamma <= 1);
	}
	return false;    // cannot solve or root is not unique
}

bool Triangle::hasSurfacePoint(const Pos &x) const
{
	double A[3][3], k[3], b[3];
	gp[0].putToArray(&A[0][0], 3);
	gp[1].putToArray(&A[0][1], 3);
	gp[2].putToArray(&A[0][2], 3);
	x.putToArray(b);

	if (Linear::Solve3D(A, b, k)) {
		return (k[0] >= 0 && k[1] >= 0 && k[1] >= 0 && fabs(k[0] + k[1] + k[2] - 1) < EPS);
	}
	return false;
//	return (fabs(x % ((gp[0] - gp[1]) ^ (gp[0] - gp[2]))) < EPS);	// three points in a plane, todo some bugs
}
