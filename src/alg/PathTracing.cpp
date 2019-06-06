//
// Created by James on 2019/4/2.
//

#include "PathTracing.h"
#include "../scene/Scene.h"
#include "../utils/sampling.h"
#include "../geometric/Sphere.h"
#include "../geometric/InfPlane.h"

using Funcs::randf;

PathTracing::PathTracing(Scene &scene_, size_t max_depth_) :
		Algorithm(scene_), max_depth(max_depth_)
{
}

Color PathTracing::_radiance(const Ray &ray, size_t depth, bool flag) const
{
	// calculate intersection:
	Intersection isect;
	if (!scene.intersectAny(ray, isect)) return Color::BLACK; // if miss, return black
	if (isect.hit->geo->type() != Geometry::SPHERE)
		flag = true;	// since we deal only with sphere

	Color color = isect.getColor();    // get texture from color
	real P = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim:
	if (depth >= max_depth || P < EPS) {    // depth limit
		if (depth < (2 * max_depth) && WITH_PROB(P)) color /= P;
		else return isect.getEmission(); // R.R. the darker the obj is, the more likely to stop radiating
	}

	Ray r_in(isect.pos, ray.dir);    // move the L1 to start from intersection point

	// compute total incoming radiance:
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
		Ray r_out(r_in.org + nl * EPS, ex * samp.x + ey * samp.y + ez * samp.z);
		I_in += _radiance(r_out, depth + 1, false);

		/** todo specially sample light sources:
		 * for all sphere light sources
		 * sphere-cap-ly sample a ray from current intersection
		 * make sure the ray doesn't hit another object, even if that is a light source too.
		 * take the hit light source's emission into I_in
		 */
		for (const Object *ls : scene.getLightSources()) {
			if (ls->geo->type() != Geometry::SPHERE)
				continue;	// deal only with sphere
			auto s = (Sphere *)ls->geo;
			Pos OS = s->c - r_in.org;
			real dist = OS.norm();
			Dir sz = OS / dist, sx, sy;
			sz.getOrthogonalBasis(sx, sy);
			real sin_theta_max = s->rad / dist;
			if (sin_theta_max > 1)
				continue;	// unwanted sample
			real cos_theta_max = sqrtf(max2(0.f, 1 - sin_theta_max * sin_theta_max));
			auto sub_samp = Sampling::uniformOnSphereCap(cos_theta_max, {randf(), randf()});
			Ray r_sub(r_in.org + nl * EPS, sx * sub_samp.x + sy * sub_samp.y + sz * sub_samp.z);
			Intersection isect_sub;
			assert(scene.intersectAny(r_sub, isect_sub));
			if (isect_sub.hit != ls || nl % r_sub.dir <= 0)
				continue;	// a shadow ray
			I_in += ls->mtr->emi * ((nl % r_sub.dir) * 2 * (1 - cos_theta_max));
			// notice: uniform sampling, so divided by Pi already
		}
	}

	// mirror reflection, Phong model: I = ks ( V . R )^n model
	if WITH_PROB(material.spec) {
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));
		// without Phong-model:
		I_in += _radiance(r_R, depth + 1);
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
		I_in += _radiance({r_in.org + nl * EPS, R}, depth + 1) * beta;*/
	}

	// dielectric refraction component
	if WITH_PROB(material.refr) {
		Ray r_R(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2));    // reflection
		bool into = (n % nl) > 0;                // Ray from outside going in?
		real nc = 1, nt = material.n_refr, nnt = into ? nc / nt : nt / nc;
		real ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) {    // total internal reflection
			I_in += _radiance(r_R, depth + 1);
		}
		else {        // refraction and reflection
			Ray r_T(r_in.org - nl * EPS, r_in.dir * nnt - nl * (ddn * nnt + sqrtf(cos2t)));
			real a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % n);
			real R0 = a * a / (b * b), Re = R0 + (1 - R0) * powf(c, 5);
			real Tr = 1 - Re, P_ = .25f + .5f * Re, RP = Re / P_;
			real TP = Tr / (1 - P_);
			I_in += (depth > 2)
					? (WITH_PROB(P_))  // Russian roulette
					  ? _radiance(r_R, depth + 1) * RP
					  : _radiance(r_T, depth + 1) * TP
					: _radiance(r_R, depth + 1) * Re + _radiance(r_T, depth + 1) * Tr;
		}
	}

	return (flag ? isect.getEmission() : Color::BLACK) + color.mul(I_in);    // be careful of flag here
}

String PathTracing::info() const
{
	Buffer buffer;
	sprintf(buffer, "<Ray Tracing>  max depth = %ld", max_depth);
	return buffer;
}
