//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_SPHERE_H
#define PHAROSA_SPHERE_H

#include "Geometry.h"

struct Sphere : Geometry
{
	Pos pos;    // center as global crd
	const double rad, rad_2;    // radius, radius^2

	Sphere(double radius_, const Pos &pos_ = {});

	void applyTransform(TransMat mat) override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	inline bool hasSurfacePoint(const Pos &x) const override
	{ return (x - pos).sqr() - rad_2 < EPS; }

	static Sphere *acquire(const Json &json);
};

#endif //PHAROSA_SPHERE_H
