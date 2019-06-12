//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_SPHERE_H
#define PHAROSA_SPHERE_H

#include "Geometry.h"
#include "Finite.h"

struct Sphere : Geometry, Finite
{
	Pos c;    // center as global crd
	const real rad, rad_2;    // radius, radius^2

	Sphere(real radius_, const Pos &pos_ = {});

	Type type() const override
	{ return SPHERE; }

	void applyTransform(const TransMat &mat) override;

	bool intersect(const Ray &ray, real &t, Intersection &isect) const override;

	void getNormal(const Pos &pos, Dir &normal) const override
	{ normal = pos - c; }

	void getUV(const Pos &pos, real &u, real &v) const override
	{
		ElAg ea = Dir(pos - c).getEulerAngles();
		u = ea.alpha, v = ea.gamma;
	}

	void report() const override;

	real xMin() const override
	{ return c.x - rad; }

	real xMax() const override
	{ return c.x + rad; }

	real yMin() const override
	{ return c.y - rad; }

	real yMax() const override
	{ return c.y + rad; }

	real zMin() const override
	{ return c.z - rad; }

	real zMax() const override
	{ return c.z + rad; }

	Pos center() const override
	{ return c; }

	real area() const override
	{ return 4 * M_PIF * rad_2; }

	static Sphere *acquire(const Json &json);
};

#endif //PHAROSA_SPHERE_H
