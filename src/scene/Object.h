//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_OBJECT_H
#define PHAROSA_OBJECT_H

#include "../lib.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"
#include "../geometric/All.h"

// physical instance of an object, including its geometric and material attributes
struct Object
{
	enum ReflType
	{
		DIFF, SPEC, REFR
	};    // diffusive, reflective, refractive

	Geometry *geo;    // geometric attributes
	Color color;
	Emission emi;    // emission
	ReflType reft;    // reflection type

	// todo texture

	template<class GeometryType>
	Object(GeometryType &&geo_, const Color &color_ = Color::WHITE, const Emission &emission_ = Emission::NONE,
		   ReflType refl_type_ = DIFF);    // init from a temporarily defined Geometry, copy it to stay long todo use move

	~Object();
};

#include "Object.tcc"

#endif //PHAROSA_OBJECT_H
