
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"
#include "../scene/Object.h"
#include "../scene/Intersection.hpp"

InfPlane::InfPlane(const Dir &n_, const Pos &p_) :
		p(p_), n(n_), D(-(p % n))
{
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}

InfPlane::InfPlane() : InfPlane(Dir::Z_AXIS, Pos::ORIGIN)
{
}

void InfPlane::applyTransform(TransMat mat)
{
	p = mat * p;
	n = mat.rot * n;
	D = -(p % n);
	Dir ex, ey;
	n.getOrthogonalBasis(ex, ey);
	cu = ex, cv = ey;
}

bool InfPlane::intersect(const Ray &ray, double &t, Intersection &isect) const
{
	// solve (L1.org + t L1.dir - c) % normal == 0
	double dn = ray.dir % n;
	if (fabs(dn) < EPS) return false;
	double ti = -(D + ray.org % n) / dn;
	if (ti < EPS || ti >= t) return false;
	t = ti;	// update
	return true;
}
