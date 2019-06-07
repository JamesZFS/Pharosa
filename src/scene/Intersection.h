//
// Created by James on 2019/5/15.
//

#ifndef PHAROSA_INTERSECTION_H
#define PHAROSA_INTERSECTION_H

#include "../Pharosa.h"
#include "Object.h"

// struct for describing surface intersection
struct Intersection
{
	const Object *hit;    // hit which object
	Pos pos;	// intersection point
	Dir normal;	// intersection normal
	real u, v;    // (u, v) coordinate

	Intersection() : hit(nullptr), u(0), v(0)
	{};

	// color getter
	inline const Color &getColor() const    // get texture color at pos
	{ return hit->mtr->texture ? hit->mtr->textureAt(u, v) : hit->mtr->color; }

	inline const Color &getEmission() const    // get texture color at pos
	{ return hit->mtr->emi; }

	inline void complementData()
	{
		hit->geo->getNormal(pos, normal);
		if (hit->mtr->texture) hit->geo->getUV(pos, u, v);
	}

	enum ScatterType {
		DIFFUSE,
		SPECULAR,
		REFRACTION
	};
	// generate a new sample ray from r_in, returns the out ray and the weight for that sample
	// return: the scattering type
	ScatterType scatter(const Ray &r_in, Ray &r_out, real &w_out) const;
};

#endif //PHAROSA_INTERSECTION_H
