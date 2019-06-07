//
// Created by James on 2019/6/7.
//

#include "PTF.h"
#include "../scene/Scene.h"
#include "../utils/sampling.h"
#include "../geometric/Sphere.h"
#include "../geometric/InfPlane.h"

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

// forward version of Path Tracing Explicit, without branching or recursion
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
		if (isect.hit->geo->type() != Geometry::SPHERE)
			flag = true;    // since we deal only with sphere
		if (flag)
			L += beta.mul(isect.getEmission());
		Color color = isect.getColor();    // get texture from color
		real P = color.max();    // select max color component
		// RR. to stop tracing at proper time
		if (depth >= max_depth || P < EPS) {    // depth | color limit
			if (depth < DEPTH_LIMIT && WITH_PROB(P)) color /= P;
			else break;
		}
		beta *= color;    // accumulate throughput

		// update incoming ray origin
		real w_out;
		Ray r_out;
		// sample one new ray
		auto scatter_type = isect.scatter(r, r_out, w_out);

		/** specially sample light sources:
		 * for all sphere light sources
		 * sphere-cap-ly sample a ray from current intersection
	 	 * make sure the ray doesn't hit another object, even if that is a light source too.
		 * take the hit light source's emission into I_in
		 */
		if (scatter_type == Intersection::DIFFUSE) {
			flag = false;
			Dir &n = isect.normal;
			Dir nl = n % r.dir < 0 ? n : -n; // regularized normal, against r direction
			for (const Object *ls : scene.getLightSources()) {
				if (ls->geo->type() != Geometry::SPHERE)
					continue;    // deal only with sphere
				auto s = (Sphere *) ls->geo;
				Pos OS = s->c - isect.pos + nl * EPS;
				real dist = OS.norm();
				Dir sz = OS / dist, sx, sy;
				sz.getOrthogonalBasis(sx, sy);
				real sin_theta_max = s->rad / dist;
				if (sin_theta_max > 1)
					continue;    // unwanted sample
				real cos_theta_max = sqrtf(max2(0.f, 1 - sin_theta_max * sin_theta_max));
				auto sub_samp = Sampling::uniformOnSphereCap(cos_theta_max, {randf(), randf()});
				Ray r_sub(isect.pos, sx * sub_samp.x + sy * sub_samp.y + sz * sub_samp.z);
				r_sub.offset(EPS);
				Intersection isect_sub;
				if (!scene.intersectAny(r_sub, isect_sub)) {    // some precision bug
					++__counter__;
					continue;
				}
				if (isect_sub.hit != ls || nl % r_sub.dir <= 0)
					continue;    // a shadow ray
				L += beta.mul(ls->mtr->emi * ((nl % r_sub.dir) * 2 * (1 - cos_theta_max)));
				// notice: uniform sampling, so divided by Pi already
			}
		}
		else
			flag = true;

		r = r_out;
		beta *= w_out;
		// next iter...
	}
	return L;
}
