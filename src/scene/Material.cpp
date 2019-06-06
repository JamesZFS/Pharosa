//
// Created by James on 2019/4/26.
//

#include "Material.h"
#include "../utils/funcs.hpp"
#include "../utils/sampling.h"

using Funcs::randf;
using Funcs::randfNormal;

Material::Material() :
		color(Color::WHITE), emi(Emission::NONE),
		diff(1), spec(0), refr(0), n_refr(1.5),
		texture(nullptr),
		Auu(1), Auv(0), Auc(0), Avu(0), Avv(1), Avc(0)
{
}

// !! notice normal is pointing outside
void Material::scatter(const Ray &r_in, const Dir &normal, size_t depth, List<Ray> &r_outs, List<real> &w_outs) const
{
	Dir nl = normal % r_in.dir < 0 ? Pos(normal) : -normal;    // regularized normal, against r_in direction

	// diffusive reflection, todo use I = kd ( L . N ) model
	Dir ex, ey;
	const Dir &ez = nl;
	ez.getOrthogonalBasis(ex, ey);
	if WITH_PROB(diff) {

//		int n_samp = max2(1, int(2 / depth));    // todo
		int n_samp = 1;
		real inv_n_samp = 1.0f / n_samp;
		for (int i = 0; i < n_samp; ++i) {
			// importance sampling cosine dist:
			auto samp = Sampling::cosineOnHemisphere({randf(), randf()});
			r_outs.emplace_back(r_in.org + nl * EPS, Dir(ex * samp.x + ey * samp.y + ez * samp.z));
			w_outs.push_back(inv_n_samp);
			// naive sampling:
//			auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
//			r_outs.emplace_back(r_in.org + nl * EPS, Dir(ex * samp.x + ey * samp.y + ez * samp.z));
//			real theta = atan2f(sqrtf(samp.x * samp.x + samp.y * samp.y), samp.z);
//			w_outs.push_back(inv_n_samp * cosf(theta) * 2);
		}
	}

	// mirror reflection, Phong model: I = ks ( V . R )^n model
	if WITH_PROB(spec) {
		// without Phong-model:
//		r_outs.emplace_back(
//				r_in.org + nl * EPS,
//				r_in.dir - nl * (nl % r_in.dir * 2));
//		w_outs.push_back(1.0);
		// naive sampling of Phong-model:
		auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
		Dir V = r_in.dir - nl * (nl % r_in.dir * 2);
		Dir R = ex * samp.x + ey * samp.y + ez * samp.z;
		r_outs.emplace_back(r_in.org + nl * EPS, R);
		// n = 2:
//		w_outs.push_back(powf(V % R, 2) * 3);
		// n = 3:
//		real alpha = Dir(V.getCoordAs(ex, ey, ez)).getEulerAngles().gamma;
//		real K = 16 / (cosf(alpha) * (5 - cosf(2 * alpha)));
//		w_outs.push_back(powf(V % R, 3) * K);
		// n = 4:
		w_outs.push_back(powf(V % R, 4) * 5);
	}

	// dielectric refraction, normal will be used instead of nl
	if WITH_PROB(refr) {
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));    // reflection
		bool into = (normal % nl) > 0;                // Ray from outside going r_in?
		real nc = 1, nt = n_refr, nnt = into ? nc / nt : nt / nc;
		real ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) {    // Total internal reflection
			r_outs.push_back(r_R);
			w_outs.push_back(1.0);
		}
		else {
			Ray r_T(r_in.org - nl * EPS, r_in.dir * nnt - nl * (ddn * nnt + sqrtf(cos2t)));
			real a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % normal);
			real R0 = a * a / (b * b), Re = R0 + (1 - R0) * powf(c, 5);
			real Tr = 1 - Re, P = .25f + .5f * Re, RP = Re / P;
			real TP = Tr / (1 - P);
			(depth > 2)
			? (WITH_PROB(P)   // Russian roulette
			   ? r_outs.push_back(r_R), w_outs.push_back(RP)
			   : r_outs.push_back(r_T), w_outs.push_back(TP))
			: r_outs.push_back(r_R), w_outs.push_back(Re), r_outs.push_back(r_T), w_outs.push_back(Tr);
		}
	}
}
