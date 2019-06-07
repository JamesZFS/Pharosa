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

PathTracing::PathTracing(Scene &scene_, size_t max_depth_) :
		Algorithm(scene_), max_depth(max_depth_)
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

	Ray r_in(isect.pos, ray.dir);    // move the L1 to start from intersection point

	// compute total incoming radiance:
	Color L_in = {0, 0, 0};
	Dir &n = isect.normal;
	Dir nl = n % r_in.dir < 0 ? n : -n; // regularized normal, against r_in direction
	auto &material = *isect.hit->mtr;

	// update incoming ray direction using R.R.
	// assuming diff + spec + refr = 1
	P = randf();
	if ((P -= material.diff) <= 0) { // diffusive reflection
		Dir ex, ey;
		const Dir &ez = nl;
		ez.getOrthogonalBasis(ex, ey);
		// importance sampling cosine dist:
		auto samp = Sampling::cosineOnHemisphere({randf(), randf()});
		Ray r_out(r_in.org + nl * EPS, ex * samp.x + ey * samp.y + ez * samp.z);
		L_in += _radiance(r_out, depth + 1);
	}
	else if ((P -= material.spec) <= 0) { // mirror reflection, Phong model: I = ks ( V . R )^n model
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));
		// without Phong-model:
		L_in += _radiance(r_R, depth + 1);
		// naive sampling of Phong-model:
		/*auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
		Dir V = r_in.dir - nl * (nl % r_in.dir * 2);
		Dir R = ex * samp.x + ey * samp.y + ez * samp.z;
		// n = 2:
	  //  w_outs.push_back(powf(V % R, 2) * 3);
		// n = 3:
	  //  real alpha = Dir(V.getCoordAs(ex, ey, ez)).getEulerAngles().gamma;
	  //  real K = 16 / (cosf(alpha) * (5 - cosf(2 * alpha)));
	  //  w_outs.push_back(powf(V % R, 3) * K);
		// n = 4:
		auto beta = powf(V % R, 4) * 5;
		L_in += _radiance({r_in.org + nl * EPS, R}, depth + 1) * beta;*/
	}
	else { // dielectric refraction component
		assert(P <= material.refr);
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));    // reflection
		bool into = (n % nl) > 0;                // Ray from outside going in?
		real nc = 1, nt = material.n_refr, nnt = into ? nc / nt : nt / nc;
		real ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) {    // total internal reflection
			L_in += _radiance(r_R, depth + 1);
		}
		else {        // refraction and reflection
			Ray r_T(r_in.org - nl * EPS, r_in.dir * nnt - nl * (ddn * nnt + sqrtf(cos2t)));
			real a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % n);
			real R0 = a * a / (b * b), Re = R0 + (1 - R0) * powf(c, 5);
			real Tr = 1 - Re, P_ = .25f + .5f * Re, RP = Re / P_;
			real TP = Tr / (1 - P_);
			L_in += (depth > 2)
					? (WITH_PROB(P_))  // Russian roulette
					  ? _radiance(r_R, depth + 1) * RP
					  : _radiance(r_T, depth + 1) * TP
					: _radiance(r_R, depth + 1) * Re + _radiance(r_T, depth + 1) * Tr;
		}
	}

	return isect.getEmission() + color.mul(L_in);    // be careful of flag here
}

String PathTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Path Tracing>  max depth = %ld", max_depth);
	return buffer;
}
