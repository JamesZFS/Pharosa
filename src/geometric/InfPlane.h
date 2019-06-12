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
	real D;	// A x + B y + C z + D == 0, D == -n.c

	Pos cu, cv;	// cache to compute texture crd

	InfPlane();	// plane facing towards z-axis at origin

	// construct by specifying normal and point
	InfPlane(const Dir &n_, const Pos &p_);

	Type type() const override
	{ return INFPLANE; }

	void applyTransform(const TransMat &mat) override;

	bool intersect(const Ray &ray, real &t, Intersection &isect) const override;

	inline bool testPoint(const Pos &x) const
	{ return fabsf((x - p) % n) < EPS; }

	inline void getNormal(const Pos &pos, Dir &normal) const override
	{ normal = n; }

	inline void getUV(const Pos &pos, real &u, real &v) const override
	{ u = cu % pos, v = cv % pos; }

	inline int above(const Pos &x) const    // +1, -1, 0 (above plane)
	{ return Funcs::sgn((x - p) % n); }

	void report() const override;

	static InfPlane *acquire(const Json &json);
};

#endif //PHAROSA_INFPLANE_H
