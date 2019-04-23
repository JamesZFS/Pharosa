//
// Created by James on 2019/4/19.
//

#include "Object.h"
#include "../geometric/All.h"

Object::Object(Geometry *geo_, const Color &color_, const Emission &emission_, Object::ReflType refl_type_) :
		geo(geo_), color(color_), emi(emission_), reft(refl_type_)
{
}

Object::~Object()
{
	delete geo;
}
