//
// Created by James on 2019/4/26.
//

#ifndef PHAROSA_MATERIAL_HPP
#define PHAROSA_MATERIAL_HPP


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
	real diff;
	real spec;
	real refr;
	real n_refr;	// refraction ratio

	Image *texture; // texture ppm
	real Auu, Auv, Auc, Avu, Avv, Avc;	// transform matrix of (u, v)

	Material() :	// dark white pure diffusive without texture
			color(Color::WHITE), emi(Emission::NONE),
			diff(1), spec(0), refr(0), n_refr(1.5),
			texture(nullptr),
			Auu(1), Auv(0), Auc(0), Avu(0), Avv(1), Avc(0)
	{
	}

	inline const Color &textureAt(real u, real v) const
	{ return texture->get(Auu * u + Auv * v + Auc, Avu * u + Avv * v + Avc); }

	inline void report() const
	{
		printf("color: ");
		color.report(false);
		printf(" emission: ");
		emi.report(true);
		printf("diff = %f, spec = %f, refr = %f, n_refr = %f\n", diff, spec, refr, n_refr);
	}

	static Material *acquire(const Json &json);
};


#endif //PHAROSA_MATERIAL_HPP
