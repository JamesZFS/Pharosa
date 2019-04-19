//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(double radius_, const Pos &pos_, ElAg euler_angles_) :
		Geometry(pos_, euler_angles_), p(pos_), gp(pos_), rad(radius_), rad_2(radius_ * radius_)
{
}

void Sphere::applyTransform()
{
	gp = mat * p;
	debug(">>> trans shpere : ");
	gp.report(true);
}

bool Sphere::intersect(const Ray &ray, double &t) const
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	Pos op = gp - ray.org;
	double b = op % ray.dir, det = b * b - op.sqr() + rad * rad;
	if (det < 0) return false;    // no solution
	else det = sqrt(det);

	return ((t = b - det) < EPS	// intersect at front of sphere?
			? ((t = b + det) > EPS) // intersect at back?
			: true);
}

Dir Sphere::normalAt(const Pos &x) const
{
	return x - gp;    // from center to x
}
