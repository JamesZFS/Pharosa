//
// Created by think on 2019/4/2.
//

#include "Object.h"

template<class GeometryType>
Object::Object(GeometryType &&geo_, const Color &color_, const Emission &emission_, ReflType refl_type_) :
		geo(new GeometryType(geo_)), color(color_), emi(emission_), reft(refl_type_)
{
}
