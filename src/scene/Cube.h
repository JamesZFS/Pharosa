//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Object.h"
#include "InfPlane.h"

struct Cube : Object
{
	InfPlane slab[3][2];	// 3 slabs, 6 planes, each slap is two paralleled planes
	Pos p[3][2];			// local crd of 6 planes

	Cube(Dir n_[3], const Pos p_[3][2], const Pos &pos_, const Color &color_,
		 const Emission &emission_ = {0, 0, 0}, const ElAg &euler_angles_ = {0, 0, 0},
		 Object::ReflType refl_type_ = Object::DIFF);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_CUBE_H
