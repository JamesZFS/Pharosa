//
// Created by think on 2019/4/2.
//

#ifndef RAYTRACKER_SPHERE_H
#define RAYTRACKER_SPHERE_H

#include "Object.h"

struct Sphere : public Object
{
	double rad;

	Sphere(const Pos &pos_, double radius_, const Color &color_, const Emission &emission_ = {0, 0, 0},
		   const ElAg &euler_angles_ = {0, 0, 0}, ReflType refl_type_ = DIFF);

	void applyTransform() override;

	bool intersect(const Ray &ray, double &t) const override;

	Dir normalAt(const Pos &x) const override;
};

#include "Sphere.cpp"

#endif //RAYTRACKER_SPHERE_H
