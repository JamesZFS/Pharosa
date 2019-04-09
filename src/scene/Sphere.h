//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_SPHERE_H
#define PHAROSA_SPHERE_H

#include "Object.h"

struct Sphere : public Object
{
	double rad;

	Sphere(double radius_, const Pos &pos_, const Color &color_, const Emission &emission_ = {0, 0, 0},
		   ElAg euler_angles_ = {0, 0, 0}, ReflType refl_type_ = DIFF);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;
};

#endif //PHAROSA_SPHERE_H
