
//
// Created by James on 2019/4/5.
//
#include "InfPlane.h"


InfPlane::InfPlane(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
				   Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_), n(0, 0, 1)
{
	applyTransform();
}

void InfPlane::applyTransform()
{
	n.rotate(ea);
}

bool InfPlane::intersect(const Ray &ray, double &t) const
{
	// solve (ray.org + t ray.dir - pos) % normal == 0
	t = (pos - ray.org) % n / (ray.dir % n);
	return (t > EPS);
}

Dir InfPlane::normalAt(const Pos &x) const
{
	return n;
}
