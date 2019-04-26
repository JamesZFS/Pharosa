//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_H
#define PHAROSA_MATERIAL_H


#include "../core/Vec.h"
#include "../core/Ray.hpp"

// physical mtr of an object
struct Material
{
	enum ReflType
	{
		DIFF, SPEC, REFR, NONE
	};

	Color color;
	Emission emi;
	ReflType reft;    // diffusive, reflective, refractive, todo we will implement BRDF here later

	// todo texture

	Material();	// dark white DIFF

	void BRDF(const Ray &in, const Dir &normal, Ray &out);

	static Material *acquire(const Json &json);
};


#endif //PHAROSA_MATERIAL_H
