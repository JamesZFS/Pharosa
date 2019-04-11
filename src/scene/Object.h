//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_OBJECT_H
#define PHAROSA_OBJECT_H

#include "../lib.h"
#include "../core/Vec.h"
#include "../core/Mat.h"
#include "../core/Ray.hpp"

// base class of 3D object
struct Object
{
	enum ReflType
	{
		DIFF, SPEC, REFR
	};    // diffusive, reflective, refractive

	Pos pos;        // ref point position in global coordinate system
	Color color;        // color
	Emission emi;    // emission
	ElAg ea;        // Euler angles
	ReflType reft;    // reflection type
	TransMat mat;	// transform matrix

	// other params ...

	Object();

	Object(const Pos &pos_, const Color &color_, const Emission &emission_ = {0, 0, 0},
		   const ElAg &euler_angles_ = {0, 0, 0}, ReflType refl_type_ = DIFF);

	virtual ~Object() = default;

	// 3D transformation, return *this
	Object &replace(const Pos &pos_, const ElAg &euler_angles_);

	Object &translate(const Pos &delta);

	virtual Object &rotate(const ElAg &dea);

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

	// determine if a point is on the surface of object
	virtual bool hasSurfacePoint(const Pos &x) const = 0;

};

#endif //PHAROSA_OBJECT_H
