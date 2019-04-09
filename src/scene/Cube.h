//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Object.h"
#include "InfPlane.h"

struct Cube : Object
{
	InfPlane f[6];	// 6 faces

	Cube(const InfPlane f_[6], const Pos &pos_, const Color &color_, const Emission &emission_,
		 const ElAg &euler_angles_, Object::ReflType refl_type_);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;
};


#endif //PHAROSA_CUBE_H
