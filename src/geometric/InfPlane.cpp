
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"
#include "../scene/Object.h"

InfPlane::InfPlane() : InfPlane(Pos::ORIGIN, ElAg::NONROT)
{
}

InfPlane::InfPlane(const Pos &p_, const ElAg &euler_angles_) :
		Geometry(p_, euler_angles_), n_org(Dir::Z_AXIS)
{
	applyTransform();
}

InfPlane::InfPlane(const Dir &n_, const Pos &p_) :
		Geometry(p_, n_.getEulerAngles()), p(p_), n_org(n_)
{
}

void InfPlane::applyTransform()
{
	p = mat.tra;
	n = mat.rot * n_org;
}
