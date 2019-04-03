//
// Created by think on 2019/4/2.
//

#include "Sphere.h"

Sphere::Sphere(Pos pos_, double radius_, Color color_, Emission emission_, ElAg euler_angles_,
			   Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_), rad(radius_)
{
}

double Sphere::intersect(const Ray &ray) const
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	Pos op = pos - ray.org;
	double t, b = op % ray.dir, det = b * b - op.sqr() + rad * rad;
	if (det < 0) return 0;
	else det = sqrt(det);
	return (t = b - det) > EPS ? t : ((t = b + det) > EPS ? t : -1);
}

void Sphere::applyTransform()
{
	// do nothing
}
