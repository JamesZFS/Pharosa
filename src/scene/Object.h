//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_OBJECT_H
#define PHAROSA_OBJECT_H

#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Mat.h"
#include "../geometric/Geometry.h"

// physical instance of an object, including its geometric and material attributes
struct Object
{
	enum ReflType
	{
		DIFF, SPEC, REFR, NONE
	};    // diffusive, reflective, refractive

	Geometry *geo;    // geometric attributes
	Color color;
	Emission emi;    // emission
	ReflType reft;    // reflection type

	// todo texture

	Object(Geometry *geo_, const Color &color_ = Color::WHITE, const Emission &emission_ = Emission::NONE,
		   ReflType refl_type_ = DIFF);

	template<class GeometryType> //todo delete
	Object(GeometryType &&geo_, const Color &color_ = Color::WHITE, const Emission &emission_ = Emission::NONE,
		   ReflType refl_type_ = DIFF);    // init from a temporarily defined Geometry, copy it to stay long todo use move

	Object(const Json &json);	// from json

	~Object();
};

template<class GeometryType>
Object::Object(GeometryType &&geo_, const Color &color_, const Emission &emission_, ReflType refl_type_) :
		geo(new GeometryType(geo_)), color(color_), emi(emission_), reft(refl_type_)
{
}

typedef List<Object *> ObjectList;

#endif //PHAROSA_OBJECT_H
