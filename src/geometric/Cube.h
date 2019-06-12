//
// Created by James on 2019/4/9.
//

#ifndef PHAROSA_CUBE_H
#define PHAROSA_CUBE_H

#include "Geometry.h"
#include "InfPlane.h"

// Parallelepiped
struct Cube : Geometry
{
	Arr2D<InfPlane, 3, 2> slab;    // 3 slabs, 6 planes, each slap is two paralleled planes
	Pos ox, oy, oz, o;

	Cube();    // unit cube

	// init from 3 basis (ox, oy, oz. by default) and left-bottom-front most point c
	Cube(const Pos &ox_, const Pos &oy_, const Pos &oz_, const Pos &o_ = {});

	// init an orthogonal cube
	Cube(real length, real width, real height, const Pos &pos = {});

	Type type() const override
	{ return CUBE; }

	void applyTransform(const TransMat &mat) override;

	bool intersect(const Ray &ray, real &t, Intersection &isect) const override;

	void getNormal(const Pos &pos, Dir &normal) const override;

	inline void getUV(const Pos &pos, real &u, real &v) const override
	{ slab[0][0].getUV(pos, u, v); }

	void report() const override;

	static Cube *acquire(const Json &json);
};

#endif //PHAROSA_CUBE_H
