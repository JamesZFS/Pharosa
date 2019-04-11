//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(double radius_, const Pos &pos_, const Color &color_, const Emission &emission_, ElAg euler_angles_,
			   ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_), rad(radius_), rad_2(radius_ * radius_)
{
}

void Sphere::applyTransform()
{
	// texture rotation?
}

bool Sphere::intersect(const Ray &ray, double &t) const
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	Pos op = pos - ray.org;
	double b = op % ray.dir, det = b * b - op.sqr() + rad * rad;
	if (det < 0) return false;    // no solution
	else det = sqrt(det);

	return ((t = b - det) < EPS	// intersect at front of sphere?
			? ((t = b + det) > EPS) // intersect at back?
			: true);
}

Dir Sphere::normalAt(const Pos &x) const
{
	return x - pos;    // from center to x
}
