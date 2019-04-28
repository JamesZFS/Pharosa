//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_TRIANGLE_H
#define PHAROSA_TRIANGLE_H

#include "Geometry.h"

struct Triangle : Geometry
{
	Arr<Pos, 3> p;    // points in Global coordinate sys
	Dir n;        // normal vector, in Global coordinate sys

	Triangle(const Pos &A, const Pos &B, const Pos &C);

	void applyTransform(TransMat mat) override;    // calculate pos according to pos

	bool intersect(const Ray &ray, double &t) const override;

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	bool hasSurfacePoint(const Pos &x) const override;

	inline double xMin() const;	// left most

	inline double xMax() const; // right most

	inline double yMin() const;

	inline double yMax() const;

	inline double zMin() const;

	inline double zMax() const;

	inline Pos center() const;	// get center point

	static Triangle *acquire(const Json &json);
};

double Triangle::xMin() const
{
	return min3(p[0].x, p[1].x, p[2].x);
}

double Triangle::xMax() const
{
	return max3(p[0].x, p[1].x, p[2].x);
}

double Triangle::yMin() const
{
	return min3(p[0].y, p[1].y, p[2].y);
}

double Triangle::yMax() const
{
	return max3(p[0].y, p[1].y, p[2].y);
}

double Triangle::zMin() const
{
	return min3(p[0].z, p[1].z, p[2].z);
}

double Triangle::zMax() const
{
	return max3(p[0].z, p[1].z, p[2].z);
}

Pos Triangle::center() const
{
	return (p[0] + p[1] + p[2]) / 3;
}

#endif //PHAROSA_TRIANGLE_H
