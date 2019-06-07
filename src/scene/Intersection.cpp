//
// Created by James on 2019/6/7.
//

#include "Intersection.h"
#include "../utils/funcs.hpp"
#include "../utils/sampling.h"

using Funcs::randf;

Intersection::ScatterType Intersection::scatter(const Ray &r_in, Ray &r_out, real &w_out) const
{
	// update incoming ray direction using R.R.
	// assuming diff + spec + refr = 1
	auto P = randf();
	auto &material = *hit->mtr;
	ScatterType type;

	if ((P -= material.diff) <= 0) { // diffusive reflection
		type = DIFFUSE;
		Dir ex, ey;
		const Dir &ez = nl;
		ez.getOrthogonalBasis(ex, ey);
		// importance sampling cosine dist:
		auto samp = Sampling::cosineOnHemisphere({randf(), randf()});
		r_out = {pos, ex * samp.x + ey * samp.y + ez * samp.z};
		w_out = 1;
	}
	else if ((P -= material.spec) <= 0) { // mirror reflection, Phong model: I = ks ( V . R )^n model
		type = SPECULAR;
		r_out = {pos, r_in.dir - nl * (nl % r_in.dir * 2)};
		w_out = 1;
		// without Phong-model:
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
		type = REFRACTION;
		Ray r_R(pos, r_in.dir - nl * (nl % r_in.dir * 2));    // reflection
		bool into = (n % nl) > 0;                // Ray from outside going in?
		real nc = 1, nt = material.n_refr, nnt = into ? nc / nt : nt / nc;
		real ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) {    // total internal reflection
			r_out = r_R;
		}
		else {        // refraction and reflection
			Ray r_T(pos, r_in.dir * nnt - nl * (ddn * nnt + sqrtf(cos2t)));
			real a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % n);
			real R0 = a * a / (b * b), Re = R0 + (1 - R0) * powf(c, 5);
			real Tr = 1 - Re, P_ = .25f + .5f * Re, RP = Re / P_;
			real TP = Tr / (1 - P_);
			if WITH_PROB(P_) { // Russian roulette, debranching
				r_out = r_R;
				w_out = RP;
			}
			else {
				r_out = r_T;
				w_out = TP;
			}
		}
	}
	r_out.offset(EPS);
	return type;
	// also returns r_out and weight
}
