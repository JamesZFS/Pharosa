//
// Created by James on 2019/4/5.
//

#ifndef RAYTRACKER_INFPLANE_H
#define RAYTRACKER_INFPLANE_H

#include "Object.h"

// infinitive large plane. Described by one point and normal vec
struct InfPlane : public Object
{
	Dir n;	// normal vector, in Global coordinate sys, default towards ez

	InfPlane(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			 ReflType refl_type_);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

};

#include "InfPlane.cpp"

#endif //RAYTRACKER_INFPLANE_H
