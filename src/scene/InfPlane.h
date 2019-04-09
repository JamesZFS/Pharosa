//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_INFPLANE_H
#define PHAROSA_INFPLANE_H

#include "Object.h"

// infinitively large plane. Described by one point and normal vec
struct InfPlane : public Object
{
	Dir n;	// normal vector, in Global coordinate sys, default towards ez

	InfPlane() = default;

	InfPlane(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			 ReflType refl_type_);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

};

#endif //PHAROSA_INFPLANE_H
