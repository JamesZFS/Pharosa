//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_TRIANGLE_H
#define PHAROSA_TRIANGLE_H

#include "Object.h"

struct Triangle : public Object
{
	Pos p[3];    // Three points in Object coordinate sys
	Pos gp[3];    // points in Global coordinate sys
	Dir n;        // normal vector, in Global coordinate sys

	Triangle(const Pos p_[3], const Pos &pos_, const Color &color_, const Emission &emission_ = {0, 0, 0},
			 const ElAg &euler_angles_ = {0, 0, 0}, ReflType refl_type_ = DIFF);

	void applyTransform() override;    // calculate gp according to p

	bool intersect(const Ray &ray, double &t) const override;

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_TRIANGLE_H
