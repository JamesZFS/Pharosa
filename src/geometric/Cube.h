//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Geometry.h"
#include "InfPlane.h"

struct Cube : Geometry
{
	Array<Dir, 3> n;	// cache original normal
	Array2D<Pos, 3, 2> p;			// local crd of 6 planes
	Array2D<InfPlane, 3, 2> slab;	// 3 slabs, 6 planes, each slap is two paralleled planes

	Cube(Array<Dir, 3> &&n_, Array2D<Pos, 3, 2> &&p_, const Pos &pos_, const ElAg &euler_angles_ = {});

//	Cube() // todo init from 3 points

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_CUBE_H
