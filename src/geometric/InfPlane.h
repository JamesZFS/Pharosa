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
	Dir n_org, n;    // normal vector, in Global coordinate sys, default towards ez

	InfPlane();

	InfPlane(const Pos &p_, const ElAg &euler_angles_);

	// construct by specifying normal
	InfPlane(const Dir &n_, const Pos &p_);

	void applyTransform() override;

	inline bool intersect(const Ray &ray, double &t) const override // solve (ray.org + t ray.dir - pos) % normal == 0
	{
		double dn = ray.dir % n;
		return (fabs(dn) < EPS ? false : ((t = (p - ray.org) % n / dn) > EPS));
	}

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	inline bool hasSurfacePoint(const Pos &x) const override
	{ return fabs((x - p) % n) < EPS; }

	inline int relationWith(const Pos &x) const    // +1, -1, 0(on plane)
	{ return Funcs::sgn((x - p) % n); }
};

#endif //PHAROSA_INFPLANE_H
