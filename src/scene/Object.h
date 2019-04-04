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
	virtual void applyTransform() = 0;    // re-apply transform from obj crd to global crd

	/** standard intersection api.
	 * @param ray
	 * @param t : distance to first intersection point
	 * @return true if intersected else false
	 */
	virtual bool intersect(const Ray &ray, double &t) const = 0;

	// calculate normal vector at surface point x
	virtual Dir normalAt(const Pos &x) const = 0;
};

#include "Object.cpp"

#endif //RAYTRACKER_OBJECT_H
