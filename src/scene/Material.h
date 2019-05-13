//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_H
#define PHAROSA_MATERIAL_H


#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"
#include "../core/Image.h"

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

	Image *texture; // texture ppm
	double scale;	// texture_scale

	Material();	// dark white pure diffusive without texture

	void BSDF(const Ray &r_in, const Dir &normal, size_t depth, List<Ray> &r_outs, List<double> &w_outs) const;

	static Material *acquire(const Json &json);
};


#endif //PHAROSA_MATERIAL_H
