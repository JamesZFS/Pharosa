//
// Created by James on 2019/5/15.
//

#ifndef PHAROSA_INTERSECTION_HPP
#define PHAROSA_INTERSECTION_HPP

#include "../Pharosa.h"
#include "Object.h"

// struct for describing surface intersection
struct Intersection
{
	const Object *hit;	// hit what
	Pos pos;
	Dir normal;
	double u, v;	// (u, v) coordinate

	Intersection() : hit(nullptr), u(0), v(0) {};

	// color getter
	inline const Color &getColor() const    // get texture color at pos
	{
		assert(hit); // todo
		return hit->mtr->texture ? hit->mtr->textureAt(u, v) : hit->mtr->color;
	}

	inline const Color &getEmission() const    // get texture color at pos
	{
		assert(hit);
		return hit->mtr->emi;
	}

	inline void scatter(const Ray &r_in, const Dir &normal, size_t depth, List<Ray> &r_outs, List<double> &w_outs) const
	{
		assert(hit);
		hit->mtr->scatter(r_in, normal, depth, r_outs, w_outs);
	}

	inline void complementData()
	{
		hit->geo->getNormal(pos, normal);
		if (hit->mtr->texture) hit->geo->getUV(pos, u, v);
	}
};

#endif //PHAROSA_INTERSECTION_HPP
