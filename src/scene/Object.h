//
// Created by think on 2019/4/2.
//

#ifndef RAYTRACKER_OBJECT_H
#define RAYTRACKER_OBJECT_H

#include "../lib.h"
#include "../Vec.hpp"
#include "../Ray.hpp"

// base class of 3D object
struct Object
{
	enum ReflType
	{
		DIFF, REFL, REFR
	};    // diffusive, reflective, refractive

	Pos pos;        // ref point position in global coordinate system
	Color color;        // color
	Emission emi;    // emission
	ElAg ea;        // Euler angles
	ReflType reft;    // reflection type

	// other params ...

	Object(const Pos &pos_, const Color &color_, const Emission &emission_ = {0, 0, 0},
		   const ElAg &euler_angles_ = {0, 0, 0}, ReflType refl_type_ = DIFF);

	virtual ~Object() = default;

	// 3D transformation, return *this
	Object &translate(const Pos &delta);

	Object &rotate(const ElAg &dea);

	// interfaces:
	virtual double intersect(const Ray &ray) const = 0; // standard intersection api. Should return -1 if not hitting
	virtual void applyTransform() = 0;    // apply transform from obj crd to global crd
};

#include "Object.cpp"

#endif //RAYTRACKER_OBJECT_H
