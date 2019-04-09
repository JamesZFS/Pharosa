
//
// Created by James on 2019/4/5.
//

#include "InfPlane.h"

InfPlane::InfPlane(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
				   Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, euler_angles_, refl_type_)
{
	applyTransform();
}

InfPlane::InfPlane(const Dir &n_, const Pos &pos_, const Color &color_, const Emission &emission_,
				   Object::ReflType refl_type_) :
		Object(pos_, color_, emission_, n_.getEulerAngles(), refl_type_), n(n_)
{
}
