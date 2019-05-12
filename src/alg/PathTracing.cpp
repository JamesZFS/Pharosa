//
// Created by James on 2019/4/2.
//

#include "PathTracing.h"
#include "../scene/Scene.h"

RayTracing::RayTracing(Scene &scene_, size_t max_depth_) :
		Algorithm(scene_), max_depth(max_depth_)
{
}

Color RayTracing::radiance(const Ray &ray, size_t depth) const
{
	// calculate intersection:
	Pos x;    // hitting point
	Dir normal;
	const Object *hit = nullptr;
	if (!scene.intersectAny(ray, hit, x, normal)) return Color::BLACK; // if miss, return black

	const Object &obj = *hit;    // the hit object

	Color color = obj.mtr->color;
	double P = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim:
	if (depth >= max_depth || P < EPS) {    // depth limit
		if (depth < (max_depth << 1) && WITH_PROB(P)) color /= P;
		else return obj.mtr->emi; // R.R. the darker the obj is, the more likely to stop radiating
	}

	Ray r_in(x, ray.dir);    // move the ray to start from intersection point
	List<Ray> r_outs;
	List<double> w_outs;    // weight of each out ray

	// compute multiple out rays:
	obj.mtr->BSDF(r_in, normal, ++depth, r_outs, w_outs);

	// weighted sum up:
	Color receiving = {0, 0, 0};
	for (size_t i = 0; i < r_outs.size(); ++i) {
		receiving += radiance(r_outs[i], depth) * w_outs[i];        // ** recurse
	}
	r_outs.clear(); w_outs.clear();
	return obj.mtr->emi + color.mul(receiving);
}

String RayTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Ray Tracing>  max depth = %ld", max_depth);
	return buffer;
}
