//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_TRIANGLE_H
#define PHAROSA_TRIANGLE_H

#include "Geometry.h"

struct Triangle : Geometry
{
	const Arr<Pos, 3> p;    // Three points in Object coordinate sys
	Arr<Pos, 3> gp;    // points in Global coordinate sys
	Dir n;        // normal vector, in Global coordinate sys

	Triangle(Arr<Pos, 3> &&p_, const Pos &pos_, const ElAg &euler_angles_);

	void applyTransform() override;    // calculate gp according to p

	bool intersect(const Ray &ray, double &t) const override;

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_TRIANGLE_H
