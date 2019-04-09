//
// Created by James on 2019/4/9.
//

#include "Cube.h"

Cube::Cube(const InfPlane f_[6], const Pos &pos_, const Color &color_, const Emission &emission_,
		   const ElAg &euler_angles_, Object::ReflType refl_type_) :
		   Object(pos_, color_, emission_, euler_angles_, refl_type_)
{
	f[0] = f_[0];
	f[1] = f_[1];
	f[2] = f_[2];
	f[3] = f_[3];
	f[4] = f_[4];
	f[5] = f_[5];
}

void Cube::applyTransform()
{

}

bool Cube::intersect(const Ray &ray, double &t) const
{
	return false;
}

Dir Cube::normalAt(const Pos &x) const
{
	return Dir();
}

