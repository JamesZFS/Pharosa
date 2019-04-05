//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_MESHOBJ_H
#define PHAROSA_MESHOBJ_H

#include "Object.h"
#include "Triangle.h"

typedef List<Triangle *> TriangleGroup;

struct MeshObj : public Object
{
	TriangleGroup meshes;	// triangle mesh segments

	MeshObj(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			ReflType refl_type_);

	void fromObj(const String &obj_path);	// append mesh segments from a .obj file

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;
};

#include "MeshObj.cpp"

#endif //PHAROSA_MESHOBJ_H
