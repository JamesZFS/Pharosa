//
// Created by James on 2019/4/10.
//

#include "Geometry.h"

Geometry::Geometry(const Pos &pos_, const ElAg &euler_angles_) : mat(pos_, euler_angles_)
{
}

Geometry &Geometry::replace(const Pos &pos_, const ElAg &euler_angles_)
{
	mat = TransMat(pos_, euler_angles_);
	applyTransform();
	return *this;
}

Geometry &Geometry::translate(const Pos &delta)
{
	mat = TransMat(delta) * mat;	// translation as the last operation
	applyTransform();
	return *this;
}

Geometry &Geometry::rotate(const ElAg &dea)
{
	mat *= TransMat(dea);
	applyTransform();
	return *this;
}
