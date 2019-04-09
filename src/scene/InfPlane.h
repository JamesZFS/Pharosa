//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_INFPLANE_H
#define PHAROSA_INFPLANE_H

#include "Object.h"
#include "../utils/funcs.hpp"

// infinitively large plane. Described by one point and normal vec
struct InfPlane : public Object
{
	Dir n;	// normal vector, in Global coordinate sys, default towards ez

	InfPlane() = default;

	InfPlane(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			 ReflType refl_type_);

	// construct by specifying normal
	InfPlane(const Dir &n_, const Pos &pos_, const Color &color_, const Emission &emission_,
			 ReflType refl_type_);

	Object &rotate(const ElAg &dea) override;

	inline void applyTransform() override
	{ (n = Dir::Z_AXIS).rotate(ea); }

	inline bool intersect(const Ray &ray, double &t) const override // solve (ray.org + t ray.dir - pos) % normal == 0
	{
		double dn = ray.dir % n;
		return (fabs(dn) < EPS ? false : ((t = (pos - ray.org) % n / dn) > EPS));
	}

	inline Dir normalAt(const Pos &x) const override
	{ return n; }

	inline bool hasSurfacePoint(const Pos &x) const override
	{ return fabs((x - pos) % n) < EPS; }

	inline int relationWith(const Pos &x) const	// +1, -1, 0(on plane)
	{ return Funcs::sgn((x - pos) % n); }
};

#endif //PHAROSA_INFPLANE_H
