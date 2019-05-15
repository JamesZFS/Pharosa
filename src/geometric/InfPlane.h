//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_INFPLANE_H
#define PHAROSA_INFPLANE_H

#include "Geometry.h"
#include "../utils/funcs.hpp"

// infinitively large plane. Described by one point and normal vec
struct InfPlane : Geometry
{
	Pos p; 	// a point on infplane
	Dir n;    // normal vector, in Global coordinate sys, default towards ez (A, B, C)
	double D;	// A x + B y + C z + D == 0, D == -n.c

	Pos cu, cv;	// cache to compute texture crd

	InfPlane();	// plane facing towards z-axis at origin

	// construct by specifying normal and point
	InfPlane(const Dir &n_, const Pos &p_);

	void applyTransform(TransMat mat) override;

	inline bool intersect(const Ray &ray, double &t) const override // solve (ray.org + t ray.dir - c) % normal == 0
	{
		double dn = ray.dir % n;
		return (fabs(dn) < EPS ? false : ((t = -(D + ray.org % n) / dn) > EPS));
	}

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	inline bool testPoint(const Pos &x) const
	{ return fabs((x - p) % n) < EPS; }

	inline int above(const Pos &x) const    // +1, -1, 0 (above plane)
	{ return Funcs::sgn((x - p) % n); }

	inline void getUV(const Pos &pos, double &u, double &v) override
	{ u = cu % pos, v = cv % pos; }

	static InfPlane *acquire(const Json &json);
};

#endif //PHAROSA_INFPLANE_H
