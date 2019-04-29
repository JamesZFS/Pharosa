//
// Created by James on 2019/4/26.
//

#include "Material.h"
#include "../utils/funcs.hpp"


Material::Material() : color(Color::WHITE), emi(Emission::NONE), reft(DIFF)
{

}

// !! notice normal is pointing outside
void Material::BRDF(const Ray &r_in, Dir &normal, size_t depth, List<Ray> &r_outs, List<double> &w_outs) const
{
	using Funcs::randf;

	Dir nl = normal % r_in.dir < 0 ? normal : -normal;    // regularized normal, against r_in direction

	switch (reft) {

		// Ideal diffusive reflection, todo use I = kd ( L . N ) model
		case Material::DIFF: {
			double r1 = randf(2 * M_PI), r2 = randf(), r2s = sqrt(r2);
			Dir ex, ey;
			const Dir &ez = nl;
			ez.getOrthogonalBasis(ex, ey);

			Dir d = ex * cos(r1) * r2s + ey * sin(r1) * r2s + ez * sqrt(1 - r2);
			r_outs.emplace_back(Ray(r_in.org + nl * EPS, d));
			w_outs.emplace_back(1.0);
			return;
		}

			// Ideal mirror reflection, todo use I = ks ( V . R )^n model
		case Material::SPEC: {
			r_outs.emplace_back(Ray(r_in.org + nl * EPS, r_in.dir - nl * (nl % r_in.dir * 2)));
			w_outs.emplace_back(1.0);
			return;
		}

			// Ideal dielectric refraction, normal will be used instead of nl
		case Material::REFR: {
			Ray r_R(r_in.org + nl * EPS, r_in.dir - normal * 2 * (normal % r_in.dir));    // reflection
			bool into = (normal % nl) > 0;                // Ray from outside going r_in?
			double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
			double ddn = r_in.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			if (cos2t < 0) {    // Total internal reflection
				r_outs.push_back(r_R);    // only reflection term
				w_outs.push_back(1.0);
				return;
			}

			Ray r_T = Ray(r_in.org - nl * EPS, r_in.org * nnt - nl * (ddn * nnt + sqrt(cos2t))); // refraction
			double a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_T.dir % normal);
			double R0 = a * a / (b * b), Re = R0 + (1 - R0) * pow(c, 5);
			double Tr = 1 - Re, P = .25 + .5 * Re;
			double RP = Re / P, TP = Tr / (1 - P);
			(depth > 4)
			? WITH_PROB(P)   // Russian roulette
			  ? r_outs.push_back(r_R), w_outs.push_back(RP)
			  : r_outs.push_back(r_T), w_outs.push_back(TP)
			: r_outs.push_back(r_R), w_outs.push_back(Re), r_outs.push_back(r_T), w_outs.push_back(Tr);
			return;
		}

		default: {	// exception
			TERMINATE("Warning: got invalid reft value \"%d\", rendering as black.\n", reft);
		}
	}
}
