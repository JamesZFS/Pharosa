//
// Created by think on 2019/4/2.
//

#include "Object.h"

Object::Object(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			   ReflType refl_type_) :
		pos(pos_), color(color_), emi(emission_), ea(euler_angles_), reft(refl_type_)
{
}

Object &Object::translate(const Pos &delta)
{
	pos += delta;
	return *this;
}

Object &Object::rotate(const ElAg &dea)
{
	ea += dea;
	applyTransform();
	return *this;
}
