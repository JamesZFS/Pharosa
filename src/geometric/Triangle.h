//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_TRIANGLE_H
#define PHAROSA_TRIANGLE_H

#include "Geometry.h"

struct Triangle : Geometry
{
	Arr<Pos, 3> p;    // Three points in Object coordinate sys
	Arr<Pos, 3> gp;    // points in Global coordinate sys
	Dir n;        // normal vector, in Global coordinate sys

	Triangle(Arr<Pos, 3> &&p_, const Pos &pos_, const ElAg &euler_angles_ = {});

	Triangle(const Json &json);

	void applyTransform() override;    // calculate gp according to p

	bool intersect(const Ray &ray, double &t) const override;

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	bool hasSurfacePoint(const Pos &x) const override;

	inline double xMin() override;	// left most

	inline double xMax() override; // right most

	inline double yMin() override;

	inline double yMax() override;

	inline double zMin() override;

	inline double zMax() override;

	inline void translateLocal(const Pos &delta) override
	{
		p[0] += delta;
		p[1] += delta;
		p[2] += delta;
	}
};

double Triangle::xMin()
{
	return min3(gp[0].x, gp[1].x, gp[2].x);
}

double Triangle::xMax()
{
	return max3(gp[0].x, gp[1].x, gp[2].x);
}

double Triangle::yMin()
{
	return min3(gp[0].y, gp[1].y, gp[2].y);
}

double Triangle::yMax()
{
	return max3(gp[0].y, gp[1].y, gp[2].y);
}

double Triangle::zMin()
{
	return min3(gp[0].z, gp[1].z, gp[2].z);
}

double Triangle::zMax()
{
	return max3(gp[0].z, gp[1].z, gp[2].z);
}

#endif //PHAROSA_TRIANGLE_H
