//
// Created by James on 2019/4/2.
//

#include "Triangle.h"
#include "../utils/Solvers.h"

Triangle::Triangle(const Pos &pos_, const Pos *p_, const Color &color_, const Emission &emission_,
				   const ElAg &euler_angles_,
				   Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_)
{
	p[0] = p_[0];
	p[1] = p_[1];
	p[2] = p_[2];
	applyTransform();
}

void Triangle::applyTransform()
{
	gp[0] = p[0].rotate(ea);
	gp[0] += pos;

	gp[1] = p[1].rotate(ea);
	gp[1] += pos;

	gp[2] = p[2].rotate(ea);
	gp[2] += pos;
}

double Triangle::intersect(const Ray &ray) const
{
	// solve Ro + t Rd = (1 - beta - gamma) P0 + beta P1 + gamma P2
	double A[3][3] = {
			{ray.dir.x, gp[0].x - gp[1].x, gp[0].x - gp[2].x},
			{ray.dir.y, gp[0].y - gp[1].y, gp[0].y - gp[2].y},
			{ray.dir.z, gp[0].z - gp[1].z, gp[0].z - gp[2].z},
	};

	double b[3], x[3];
	(gp[0] - ray.org).putToArray(b);

	if (Solver::SolveLinear((double **) A, b, x, 3)) {    // solvable and has only root
		double t = x[0], beta = x[1], gamma = x[2];
		return (t > EPS && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1 && beta + gamma <= 1) ? t : -1;
	}
	else {    // cannot solve or root is not unique
		return -1;
	}
}
