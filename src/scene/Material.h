//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_H
#define PHAROSA_MATERIAL_H


#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"

// physical mtr of an object
struct Material
{
	Color color;
	Emission emi;

	// diffusive, reflective, refractive ratio
	double diff;
	double spec;
	double refr;
	double n_refr;	// refraction ratio

	// todo texture

	Material();	// dark white pure diffusive

	void BRDF(const Ray &r_in, Dir &normal, size_t depth, List<Ray> &r_outs, List<double> &w_outs) const;

	static Material *acquire(const Json &json);
};


#endif //PHAROSA_MATERIAL_H
