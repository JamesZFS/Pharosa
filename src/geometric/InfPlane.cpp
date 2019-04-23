
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"
#include "../scene/Object.h"

InfPlane::InfPlane() : D(0)
{
}

InfPlane::InfPlane(const Pos &p_, const ElAg &euler_angles_) :
		Geometry(p_, euler_angles_)
{
	applyTransform();
}

InfPlane::InfPlane(const Dir &n_, const Pos &p_) :
		Geometry(p_, n_.getEulerAngles()), p(p_), n(n_), D(-(p % n))
{

}

void InfPlane::applyTransform()
{
	p = mat.tra;
	n = mat.rot * Dir::Z_AXIS;
	D = -(p % n);
}
