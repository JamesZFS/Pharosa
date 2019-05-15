//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_TRIANGLE_H
#define PHAROSA_TRIANGLE_H

#include "Geometry.h"
#include "Finite.h"

struct Triangle : Geometry, Finite
{
	Arr<Pos, 3> p;    // points in Global coordinate sys
	Dir n;        // normal vector, in Global coordinate sys

	Pos cu, cv;	// cache to compute texture

	Triangle(const Pos &A, const Pos &B, const Pos &C);

	void applyTransform(TransMat mat) override;    // calculate c according to c

	bool intersect(const Ray &ray, double &t, Intersection &isect) const override;

	inline void getNormal(const Pos &pos, Dir &normal) const override
	{ normal = n; }

	void getUV(const Pos &pos, double &u, double &v) const override
	{ u = cu % pos, v = cv % pos; }

	// override Finite:
	inline double xMin() const override    // left most
	{ return min3(p[0].x, p[1].x, p[2].x); }

	inline double xMax() const override // right most
	{ return max3(p[0].x, p[1].x, p[2].x); }

	inline double yMin() const override
	{ return min3(p[0].y, p[1].y, p[2].y); }

	inline double yMax() const override
	{ return max3(p[0].y, p[1].y, p[2].y); }

	inline double zMin() const override
	{ return min3(p[0].z, p[1].z, p[2].z); }

	inline double zMax() const override
	{ return max3(p[0].z, p[1].z, p[2].z); }

	inline Pos center() const override	// get center point
	{ return (p[0] + p[1] + p[2]) / 3; }

	static Triangle *acquire(const Json &json);
};

#endif //PHAROSA_TRIANGLE_H
