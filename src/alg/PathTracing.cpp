//
// Created by James on 2019/4/2.
//

#include "PathTracing.h"
#include "../scene/Scene.h"

PathTracing::PathTracing(Scene &scene_, size_t max_depth_) :
		Algorithm(scene_), max_depth(max_depth_)
{
}

Color PathTracing::radiance(const Ray &ray, size_t depth) const
{
	// calculate intersection:
	Intersection isect;
	if (!scene.intersectAny(ray, isect)) return Color::BLACK; // if miss, return black

	Color color = isect.getColor();	// get texture from color
	double P = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim:
	if (depth >= max_depth || P < EPS) {    // depth limit
		if (depth < (max_depth << 1) && WITH_PROB(P)) color /= P;
		else return isect.getEmission(); // R.R. the darker the obj is, the more likely to stop radiating
	}

	Ray r_in(isect.pos, ray.dir);    // move the L1 to start from intersection point
	List<Ray> r_outs;
	List<double> w_outs;    // weight of each out L1

	// compute multiple out rays:
	isect.scatter(r_in, isect.normal, ++depth, r_outs, w_outs);

	// weighted sum up:
	Color receiving = {0, 0, 0};
	for (size_t i = 0; i < r_outs.size(); ++i) {
		receiving += radiance(r_outs[i], depth) * w_outs[i];        // ** recurse
	}
	return isect.getEmission() + color.mul(receiving);
}

String PathTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Ray Tracing>  max depth = %ld", max_depth);
	return buffer;
}
