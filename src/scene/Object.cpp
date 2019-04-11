//
// Created by think on 2019/4/2.
//

#include "Object.h"

Object::Object()
{
	reft = DIFF;
}

Object::Object(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
			   ReflType refl_type_) :
		pos(pos_), color(color_), emi(emission_), ea(euler_angles_), reft(refl_type_), mat(pos, ea)
{
}

Object &Object::replace(const Pos &pos_, const ElAg &euler_angles_)
{
	pos = pos_;
	ea = euler_angles_;
	applyTransform();
	return *this;
}

Object &Object::translate(const Pos &delta)
{
	pos += delta;
	applyTransform();
	return *this;
}

Object &Object::rotate(const ElAg &dea)
{
	applyTransform();
	return *this;
}
