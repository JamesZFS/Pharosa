//
// Created by James on 2019/4/19.
//

#include "Object.h"
#include "../geometric/All.h"

Object::Object(Geometry *geo_, const Material *material_) : geo(geo_), mtr(material_)
{

}

Object::~Object()
{
	delete geo;
}

Object &Object::translate(const Pos &delta)
{
	geo->applyTransform(TransMat(delta));
	return *this;
}

Object &Object::rotate(const ElAg &ea)
{
	geo->applyTransform(TransMat(ea));
	return *this;
}

Object &Object::rotate(const ElAg &ea, const Pos &pos)
{
	geo->applyTransform(TransMat(ea, pos) * TransMat(-pos));
	return *this;
}
