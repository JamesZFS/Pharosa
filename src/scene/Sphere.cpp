//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(const Pos &pos_, double radius_, const Color &color_, const Emission &emission_,
		Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, ElAg::NONROT, refl_type_), rad(radius_)
{
}

void Sphere::applyTransform()
{
	// do nothing here
}

bool Sphere::intersect(const Ray &ray, double &t) const
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	Pos op = pos - ray.org;
	double b = op % ray.dir, det = b * b - op.sqr() + rad * rad;
	if (det < 0) return false;    // no solution
	else det = sqrt(det);

	t = b - det;    // intersect at front of sphere
	if (t <= EPS) {        // intersect at back of sphere
		t = b + det;
		if (t <= EPS) return false;    // intersect before ray.org
	}
	return true;
}

Dir Sphere::normalAt(const Pos &x) const
{
	return x - pos;    // from center to x
}
