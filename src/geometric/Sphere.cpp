//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(double radius_, const Pos &pos_) :
		pos(pos_), rad(radius_), rad_2(radius_ * radius_)
{
}

void Sphere::applyTransform(TransMat mat)
{
	pos = mat * pos;
}

bool Sphere::intersect(const Ray &ray, double &t) const
{
	// Solve t^2*d.d + 2*t*(o-pos).d + (o-pos).(o-pos)-R^2 = 0
	Pos op = pos - ray.org;
	double b = op % ray.dir, det = b * b - op.sqr() + rad_2;
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
