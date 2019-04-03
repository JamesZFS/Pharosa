//
// Created by think on 2019/4/2.
//

#ifndef RAYTRACKER_SPHERE_H
#define RAYTRACKER_SPHERE_H

#include "Object.h"

struct Sphere : public Object
{
	double rad;

	Sphere(Pos pos_, double radius_, Color color_, Emission emission_ = {0, 0, 0}, ElAg euler_angles_ = {0, 0, 0},
		   ReflType refl_type_ = DIFF);

	double intersect(const Ray &ray) const override;

	void applyTransform() override;
};

#include "Sphere.cpp"

#endif //RAYTRACKER_SPHERE_H
