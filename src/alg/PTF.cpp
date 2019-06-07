//
// Created by James on 2019/6/7.
//

#include "PTF.h"
#include "../scene/Scene.h"
#include "../utils/sampling.h"
#include "../geometric/Sphere.h"
#include "../geometric/InfPlane.h"

using Funcs::randf;

PTF::PTF(Scene &scene_, size_t max_depth_) : PathTracing(scene_, max_depth_)
{

}

String PTF::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Path Tracing Forward>  max depth = %ld", max_depth);
	return buffer;
}

// forward version of Path Tracing, without branching
Color PTF::radiance(const Ray &ray) const
{
	size_t depth;
	Color beta = {1, 1, 1};	// throughput
	Color L = {0, 0, 0};	// radiance
	Ray r = ray;

	for (depth = 0;; ++depth) {
		// calculate intersection:
		Intersection isect;
		if (!scene.intersectAny(r, isect)) break; // if miss, stop tracing
		L += beta.mul(isect.getEmission());
		Color color = isect.getColor();    // get texture from color
		real P = color.max();    // select max color component
		// RR. to stop tracing at proper time
		if (depth >= max_depth || P < EPS) {    // depth | color limit
			if (depth < DEPTH_LIMIT && WITH_PROB(P)) color /= P;
			else break;
		}
		beta *= color;	// accumulate throughput

		// update incoming ray origin
		r.org = isect.pos;
		Dir &n = isect.normal;
		Dir nl = n % r.dir < 0 ? n : -n; // regularized normal, against r direction
		auto &material = *isect.hit->mtr;

		// update incoming ray direction using R.R.
		// assuming diff + spec + refr = 1
		P = randf();
		if ((P -= material.diff) <= 0) { // Diffuse Reflection
			Dir ex, ey;
			const Dir &ez = nl;
			ez.getOrthogonalBasis(ex, ey);
			// importance sampling cosine dist:
			auto samp = Sampling::cosineOnHemisphere({randf(), randf()});
			r.dir = ex * samp.x + ey * samp.y + ez * samp.z;
			r.offset(EPS);
		}
		else if ((P -= material.spec) <= 0) { // Ideal Specular Reflection
			r.dir -= nl * (nl % r.dir * 2);
			r.offset(EPS);
		}
		else { // Refraction
			assert(P <= material.refr);
			Ray r_R(r.org, r.dir - nl * (nl % r.dir * 2));    // reflection
			r_R.offset(EPS);
			bool into = (n % nl) > 0;                // Ray from outside going in?
			real nc = 1, nt = material.n_refr, nnt = into ? nc / nt : nt / nc;
			real ddn = r.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
			if (cos2t < 0) {    // total internal reflection
				r = r_R;
			}
			else {        // refraction and reflection
				Ray r_T(r.org - nl * EPS, r.dir * nnt - nl * (ddn * nnt + sqrtf(cos2t)));
				real a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % n);
				real R0 = a * a / (b * b), Re = R0 + (1 - R0) * powf(c, 5);
				real Tr = 1 - Re, P_ = .25f + .5f * Re, RP = Re / P_;
				real TP = Tr / (1 - P_);
				if WITH_PROB(P_) { // Russian roulette, debranching
					r = r_R;
					beta *= RP;
				}
				else {
					r = r_T;
					beta *= TP;
				}
			}
		} // incoming ray update
	}
	return L;
}
