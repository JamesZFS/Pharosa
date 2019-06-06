//
// Created by James on 2019/4/2.
//

#include "PathTracing.h"
#include "../scene/Scene.h"
#include "../utils/sampling.h"

using Funcs::randf;

PathTracing::PathTracing(Scene &scene_, size_t max_depth_) :
		Algorithm(scene_), max_depth(max_depth_)
{
}

Color PathTracing::radiance(const Ray &ray, size_t depth) const
{
	// calculate intersection:
	Intersection isect;
	if (!scene.intersectAny(ray, isect)) return Color::BLACK; // if miss, return black

	Color color = isect.getColor();    // get texture from color
	double P = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim:
	if (depth >= max_depth || P < EPS) {    // depth limit
		if (depth < (max_depth << 1) && WITH_PROB(P)) color /= P;
		else return isect.getEmission(); // R.R. the darker the obj is, the more likely to stop radiating
	}

	Ray r_in(isect.pos, ray.dir);    // move the L1 to start from intersection point

	// compute multiple out rays:
//	isect.scatter(r_in, isect.normal, ++depth, r_outs, w_outs);

	// weighted sum up:
	Color I_in = {0, 0, 0};
	Dir &n = isect.normal;
	Dir nl = n % r_in.dir < 0 ? n : -n; // regularized normal, against r_in direction
	auto &material = *isect.hit->mtr;

	// diffusive reflection, todo use I = kd ( L . N ) model
	Dir ex, ey;
	const Dir &ez = nl;
	ez.getOrthogonalBasis(ex, ey);
	if WITH_PROB(material.diff) {
		// importance sampling cosine dist:
		auto samp = Sampling::cosineOnHemisphere({randf(), randf()});
		I_in += radiance({r_in.org + nl * EPS, ex * samp.x + ey * samp.y + ez * samp.z}, depth + 1);
	}

	// mirror reflection, Phong model: I = ks ( V . R )^n model
	if WITH_PROB(material.spec) {
		/*// without Phong-model:
		r_outs.emplace_back(
				r_in.org + nl * EPS,
				r_in.dir - nl * (nl % r_in.dir * 2));
		w_outs.push_back(1.0);*/
		// naive sampling of Phong-model:
		auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
		Dir V = r_in.dir - nl * (nl % r_in.dir * 2);
		Dir R = ex * samp.x + ey * samp.y + ez * samp.z;
		/*// n = 2:
		w_outs.push_back(pow(V % R, 2) * 3);
		// n = 3:
		double alpha = Dir(V.getCoordAs(ex, ey, ez)).getEulerAngles().gamma;
		double K = 16 / (cos(alpha) * (5 - cos(2 * alpha)));
		w_outs.push_back(pow(V % R, 3) * K);*/
		// n = 4:
		auto beta = pow(V % R, 4) * 5;
		I_in += radiance({r_in.org + nl * EPS, R}, depth + 1) * beta;
	}

	// dielectric refraction, normal will be used instead of nl
	if WITH_PROB(material.refr) {
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));    // reflection
		bool into = (n % nl) > 0;                // Ray from outside going r_in?
		double nc = 1, nt = material.n_refr, nnt = into ? nc / nt : nt / nc;
		double ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) {    // Total internal reflection
			I_in += radiance(r_R, depth);
		}
		else {		// refraction and reflection
			Ray r_T(r_in.org - nl * EPS, r_in.dir * nnt - nl * (ddn * nnt + sqrt(cos2t)));
			double a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % n);
			double R0 = a * a / (b * b), Re = R0 + (1 - R0) * pow(c, 5);
			double Tr = 1 - Re, P_ = .25 + .5 * Re, RP = Re / P_;
			double TP = Tr / (1 - P_);
			I_in += (depth > 2)
					? (WITH_PROB(P_))  // Russian roulette
					  ? radiance(r_R, depth + 1) * RP
					  : radiance(r_T, depth + 1) * TP
					: radiance(r_R, depth + 1) * Re + radiance(r_T, depth + 1) * Tr;
		}
	}

	return isect.getEmission() + color.mul(I_in);
}

String PathTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Ray Tracing>  max depth = %ld", max_depth);
	return buffer;
}
