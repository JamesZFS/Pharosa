//
// Created by James on 2019/4/2.
//

#include "../utils/funcs.hpp"
#include "RayTracing.h"

template<unsigned int MAX_DEPTH>
RayTracing<MAX_DEPTH>::RayTracing(Stage &stage_, Cameras::Camera &camera_) : Illuminator(stage_, camera_)
{
}

template<unsigned int MAX_DEPTH>
Color RayTracing<MAX_DEPTH>::radiance(const Ray &ray, unsigned int depth) const
{
	using namespace Scenes;
	using Funcs::randf;

	// calculate intersection
	Pos x;    // hitting point
	Dir normal;
	const Object *hit = nullptr;
	if (!stage.intersectAny(ray, hit, x, normal)) return Color::BLACK; // if miss, return black

	const Object &obj = *hit;    // the hit object
	Dir nl = normal % ray.dir < 0 ? normal : normal * -1;    // regularized normal, against in direction
	Color color = obj.color;    // color to render
	double p = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim
	if (++depth > MAX_DEPTH || p < EPS) {    // depth limit
		if (depth <= (MAX_DEPTH << 1) && randf() < p) color /= p;
		else return obj.emi; // R.R. the darker the obj is, the more likely to stop radiating
	}

	// recursively trace back
	// todo implement BRDF / BTDF which receives an incidence and returns a reflection
	// todo light attenuation
	switch (obj.reft) {

		// Ideal diffusive reflection, todo use I = kd ( L . N ) model
		case Object::DIFF: {
			double r1 = randf(2 * M_PI), r2 = randf(), r2s = sqrt(r2);
			Dir ex, ey;
			const Dir &ez = nl;
			ez.getOrthogonalBasis(ex, ey);

			Dir d = ex * cos(r1) * r2s + ey * sin(r1) * r2s + ez * sqrt(1 - r2);
			return obj.emi + color.mul(radiance(Ray(x, d), depth));
		}

			// Ideal mirror reflection, todo use I = ks ( V . R )^n model
		case Object::SPEC: {
			return obj.emi + color.mul(radiance(Ray(x, ray.dir - nl * 2 * (nl % ray.dir)), depth));
		}

			// Ideal dielectric refraction
		case Object::REFR: {
			Ray reflRay(x, ray.dir - normal * 2 * (normal % ray.dir));
			bool into = (normal % nl) > 0;                // Ray from outside going in?
			double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
			double ddn = ray.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			if (cos2t < 0) {    // Total internal reflection
				return obj.emi + color.mul(radiance(reflRay, depth));    // only reflection term
			}

			Dir &&d = ray.dir * nnt - nl * (ddn * nnt + sqrt(cos2t));
			Ray r_out(x, d);
			double a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_out.dir % normal);
			double R0 = a * a / (b * b), Re = R0 + (1 - R0) * pow(c, 5);
			double Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P;
			double TP = Tr / (1 - P);
			return obj.emi + color.mul(
					depth > 2
					? (randf() < P   // Russian roulette
					   ? radiance(reflRay, depth) * RP
					   : radiance(r_out, depth) * TP)
					: radiance(reflRay, depth) * Re + radiance(r_out, depth) * Tr    // weighted addition
			);
		}

		default: {
			Buffer buffer;
			sprintf(buffer, "Warning: got invalid reft value \"%d\", rendering as black.\n", obj.reft);
			warn(buffer);
			return Color::BLACK;
		}
	}
}
