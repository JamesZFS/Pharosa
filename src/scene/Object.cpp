//
// Created by think on 2019/4/2.
//

#include "Object.h"

Object::Object(Pos pos_, Color color_, Emission emission_, ElAg euler_angles_, ReflType refl_type_) :
		pos(pos_), color(color_), emi(emission_), ea(euler_angles_), reft(refl_type_)
{
}

double Object::intersect(const Ray &ray) const
{
	return -1;
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

void Object::applyTransform()
{
}
