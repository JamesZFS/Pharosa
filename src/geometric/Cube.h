//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Geometry.h"
#include "InfPlane.h"

struct Cube : Geometry
{
	const Arr<Dir, 3> n;    // cache original normal
	const Arr2D<Pos, 3, 2> p;            // local crd of 6 planes
	Arr2D<InfPlane, 3, 2> slab;    // 3 slabs, 6 planes, each slap is two paralleled planes

	Cube(Arr<Dir, 3> &&n_, Arr2D<Pos, 3, 2> &&p_, const Pos &pos_ = {}, const ElAg &euler_angles_ = {});

	// init from 3 vertices, ox, oy, oz. by default, the o in local crd sys is (0, 0, 0)
	Cube(Arr<Pos, 3> &&vertices_, const Pos &pos_ = {}, const ElAg &euler_angles_ = {});

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;

	bool hasSurfacePoint(const Pos &x) const override;
};

#endif //PHAROSA_CUBE_H
