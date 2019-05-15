//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(double radius_, const Pos &pos_) :
		c(pos_), rad(radius_), rad_2(radius_ * radius_)
{
}

void Sphere::applyTransform(TransMat mat)
{
	c = mat * c;
}

bool Sphere::intersect(const Ray &ray, double &t) const
{
	// Solve2DTol t^2*d.d + 2*t*(o-c).d + (o-c).(o-c)-R^2 = 0
	Pos op = c - ray.org;
	double b = op % ray.dir, det = b * b - op.sqr() + rad_2;
	if (det < 0) return false;    // no solution
	else det = sqrt(det);

	return ((t = b - det) < EPS	// intersect at front of sphere?
			? ((t = b + det) > EPS) // intersect at back?
			: true);
}

Dir Sphere::normalAt(const Pos &x) const
{
	return x - c;    // from center to x
}
