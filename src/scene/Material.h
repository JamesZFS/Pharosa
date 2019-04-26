//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_H
#define PHAROSA_MATERIAL_H


#include "../core/Vec.h"
#include "../core/Ray.hpp"
#include "Object.h"

// physical material of an object
struct Material
{
	Color color;
	Emission emi;
	Object::ReflType reft;    // diffusive, reflective, refractive, todo we will implement BRDF here later

	// texture

	Material();	// dark white DIFF

	Material(const Json &json);

	void BRDF(const Ray &in, const Dir &normal, Ray &out);
};


#endif //PHAROSA_MATERIAL_H
