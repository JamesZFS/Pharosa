//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_OBJECT_H
#define PHAROSA_OBJECT_H

#include "../Pharosa.h"
#include "Material.hpp"
#include "../geometric/Geometry.h"

// physical instance of an object, including its geometric and material attributes
struct Object
{
	Geometry *geo;    // geometric attributes
	const Material *mtr;    // material attributes
	const String name;	// for debug

	Object(Geometry *geo_, const Material *material_, const String &name_ = "");

	~Object();

	// 3D transformation, return *this
	Object &translate(const Pos &delta);

	Object &rotate(const ElAg &ea);

	Object &rotate(const ElAg &ea, const Pos &pos);    // rotate along c

	void report() const;
};

typedef List<Object *> ObjectList;

#endif //PHAROSA_OBJECT_H
