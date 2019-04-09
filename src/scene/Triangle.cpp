//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/solvers/Linear.h"

Triangle::Triangle(const Pos &pos_, const Pos p_[3], const Color &color_, const Emission &emission_,
				   const ElAg &euler_angles_, Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_)
{
	p[0] = p_[0];
	p[1] = p_[1];
	p[2] = p_[2];
	applyTransform();
}

void Triangle::applyTransform()
{
	gp[0] = p[0];
	gp[0].rotate(ea);
	gp[0] += pos;

	gp[1] = p[1];
	gp[1].rotate(ea);
	gp[1] += pos;

	gp[2] = p[2];
	gp[2].rotate(ea);
	gp[2] += pos;

	(n = (gp[0] - gp[1]) ^ (gp[0] - gp[2])).unitize();
}

bool Triangle::intersect(const Ray &ray, double &t) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	double M[3][4] = {
			{ray.dir.x, gp[0].x - gp[1].x, gp[0].x - gp[2].x, gp[0].x - ray.org.x},
			{ray.dir.y, gp[0].y - gp[1].y, gp[0].y - gp[2].y, gp[0].y - ray.org.y},
			{ray.dir.z, gp[0].z - gp[1].z, gp[0].z - gp[2].z, gp[0].z - ray.org.z},
	};

	if (Linear::SolveInPlace(M)) {    // solvable and has only root	todo use 3D
		t = M[0][3];
		double beta = M[1][3], gamma = M[2][3];
		return (t > EPS && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1 && beta + gamma <= 1);
		// valid intersection
	}
	// cannot solve or root is not unique
	return false;
}
