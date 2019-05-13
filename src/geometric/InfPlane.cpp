
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"
#include "../scene/Object.h"

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
