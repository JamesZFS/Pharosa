//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Geometry.h"
#include "InfPlane.h"

struct Cube : Geometry
{
	Dir n[3];	// cache original normal
	Pos p[3][2];			// local crd of 6 planes
	InfPlane slab[3][2];	// 3 slabs, 6 planes, each slap is two paralleled planes

	Cube(const Dir n_[3], const Pos p_[3][2], const Pos &pos_, const ElAg &euler_angles_);

//	Cube() // todo init from 3 points

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_CUBE_H
