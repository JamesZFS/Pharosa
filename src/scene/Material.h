//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_H
#define PHAROSA_MATERIAL_H


#include "../Pharosa.h"
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
	double Auu, Auv, Auc, Avu, Avv, Avc;	// transform matrix of (u, v)

	Material();	// dark white pure diffusive without texture

	// input r_in, normal and get r_outs, w_outs
	void scatter(const Ray &r_in, const Dir &normal, size_t depth, List<Ray> &r_outs, List<double> &w_outs) const;

	inline const Color &textureAt(double u, double v) const
	{ return texture->get(Auu * u + Auv * v + Auc, Avu * u + Avv * v + Avc); }

	static Material *acquire(const Json &json);
};


#endif //PHAROSA_MATERIAL_H
