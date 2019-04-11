//
// Created by think on 2019/4/2.
//

#include "Object.h"

Object::Object(Geometry *geo_, const Color &color_, const Emission &emission_, ReflType refl_type_) :
		geo(geo_), color(color_), emi(emission_), reft(refl_type_)
{
}
