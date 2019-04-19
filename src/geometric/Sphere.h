//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_SPHERE_H
#define PHAROSA_SPHERE_H

#include "Geometry.h"

struct Sphere : Geometry
{
	const Pos p; // center as local crd
	Pos gp;	// center as global crd
	const double rad, rad_2;	// radius, radius^2

	Sphere(double radius_, const Pos &pos_, ElAg euler_angles_ = {});

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	inline bool hasSurfacePoint(const Pos &x) const override
	{ return (x - gp).sqr() - rad_2 < EPS; }
};

#endif //PHAROSA_SPHERE_H
