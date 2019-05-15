//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_SPHERE_H
#define PHAROSA_SPHERE_H

#include "Geometry.h"

struct Sphere : Geometry
{
	Pos c;    // center as global crd
	const double rad, rad_2;    // radius, radius^2

	Sphere(double radius_, const Pos &pos_ = {});

	void applyTransform(TransMat mat) override;

	bool intersect(const Ray &ray, double &t, Intersection &isect) const override;

	void getNormal(const Pos &pos, Dir &normal) const override
	{ normal = pos - c; }

	void getUV(const Pos &pos, double &u, double &v) const override
	{
		ElAg &&ea = Dir(pos - c).getEulerAngles();
		u = ea.alpha, v = ea.gamma;
	}

	static Sphere *acquire(const Json &json);
};

#endif //PHAROSA_SPHERE_H
