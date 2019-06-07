//
// Created by James on 2019/4/2.
//

#include "PathTracing.h"
#include "../scene/Scene.h"
#include "../utils/sampling.h"
#include "../geometric/Sphere.h"
#include "../geometric/InfPlane.h"

using Funcs::randf;

const size_t PathTracing::DEPTH_LIMIT = 50;

PathTracing::PathTracing(const Scene &scene_,  Camera &camera_, size_t max_depth_) :
		Algorithm(scene_, camera_), max_depth(max_depth_)
{
}

Color PathTracing::_radiance(const Ray &ray, size_t depth) const
{
	// calculate intersection:
	Intersection isect;
	if (!scene.intersectAny(ray, isect)) return Color::BLACK; // if miss, return black

	Color color = isect.getColor();    // get texture from color
	real P = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim:
	if (depth >= max_depth || P < EPS) {    // depth limit
		if (depth < DEPTH_LIMIT && WITH_PROB(P)) color /= P;
		else return isect.getEmission(); // R.R. the darker the obj is, the more likely to stop radiating
	}

	// compute total incoming radiance:
	Ray r_out;
	real w_out;
	isect.scatter(ray, r_out, w_out);	// sample one out ray
	Color L_in = _radiance(r_out, depth + 1) * w_out;

	return isect.getEmission() + color.mul(L_in);
}

String PathTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Path Tracing>  max depth = %ld", max_depth);
	return buffer;
}
