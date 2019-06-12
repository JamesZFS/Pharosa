//
// Created by James on 2019/6/7.
//

#include "PTF.h"
#include "../scene/Scene.h"
#include "../geometric/Finite.h"

using Funcs::randf;

PTF::PTF(const Scene &scene_, Camera &camera_, size_t max_depth_) : PathTracing(scene_, camera_, max_depth_)
{

}

String PTF::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Path Tracing Forward>  max depth = %ld", max_depth);
	return buffer;
}

// forward version of Path Tracing Explicit, without branching or recursion, still bias free
Color PTF::radiance(const Ray &ray) const
{
	size_t depth;
	Color beta = {1, 1, 1};    // throughput
	Color L = {0, 0, 0};    // radiance
	Ray r = ray;
	bool flag = true;    // whether to consider self emission term in current depth

	for (depth = 0;; ++depth) {
		// calculate intersection:
		Intersection isect;
		if (!scene.intersectAny(r, isect)) break; // if miss, stop tracing

		// accumulate radiance and throughput
		if (!dynamic_cast<Finite *>(isect.hit->geo))
			flag = true;    // since we deal only with Ld of finite obj
		if (flag)
			L += beta.mul(isect.getEmission());
		Color color = isect.getColor();    // get texture from color
		real P = color.max();    // select max color component
		// RR. to stop tracing at proper time
		if (depth >= max_depth || P < EPS) {    // depth | color limit
			if (depth < DEPTH_LIMIT && WITH_PROB(P)) color /= P;
			else break;
		}
		beta *= color;

		// update incoming ray origin
		real w_out;
		Ray r_out;
		// sample one new ray
		auto scatter_type = isect.scatter(r, r_out, w_out);

		// account for direct light radiance Ld
		if (scatter_type == Intersection::DIFFUSE) {
			flag = false;
			L += beta.mul(computeLd(isect));  // faster version, higher variance
		}
		else
			flag = true;

		r = r_out;
		beta *= w_out;
		// next iter...
	}
	return L;
}
