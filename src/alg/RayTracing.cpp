//
// Created by James on 2019/4/2.
//

#include "RayTracing.h"

RayTracing::RayTracing(const Stage &stage_, Cameras::BasicCamera &camera_) : GI(stage_, camera_)
{
}

void RayTracing::render(unsigned int n_epoch)
{
	double progress;
	Ray ray;
	double t;
	Scenes::Object *obj = nullptr;
	for (unsigned int epoch = 0; epoch < n_epoch; ++epoch) {
		debug("=== epoch %d / %d ===\n", epoch, n_epoch);
		camera.resetProgress();
		while ((progress = camera.progress()) < 1.0) {
			ray = camera.shootRay();
			stage.intersectAny(ray, t, obj);
		}
	}
}

Color RayTracing::radiance(const Ray &r, unsigned int depth)
{
	return {};
//	double t;                               // distance to intersection
//	int id = 0;                               // id of intersected object
//	if (!stage.intersectAny(r, t, id)) return {}; // if miss, return black
//	const Sphere &obj = spheres[id];        // the hit object
//	Vec x = r.o + r.d * t;                    // hitting point
//	Vec n = (x - obj.p).norm();                // normal
//	Vec nl = n.dot(r.d) < 0 ? n : n * -1;
//	Vec f = obj.c;
//	double p = f.max(); // max color component as refl_t
//	if (++depth > 5 || p < EPS) {    // depth limit
//		if (randf() < p) f = f * (1 / p);    // brighter?
//		else return obj.e; //R.R.	the darker, the more likely to stop radiating
//	}
//	switch (obj.refl_t) {
//		case DIFF: {           // Ideal DIFFUSE reflection
//			double r1 = 2 * M_PI * randf(), r2 = randf(), r2s = sqrt(r2);
//			Vec k = nl;
//			Vec i = ((fabs(k.x) > .1 ? Vec(0, 1) : Vec(1)) ^ k).norm();    // .1 is the max threshold value for k.x
//			Vec j = k ^i;    // i, j, k(nl) coordinate system
////			auto [i, j] = k.get_orthogonal_axises();
//			Vec d = (i * cos(r1) * r2s + j * sin(r1) * r2s + k * sqrt(1 - r2)).norm();
//			return obj.e + f.mult(radiance(Ray(x, d), depth));
//		}
//		case SPEC: {         // Ideal SPECULAR reflection
//			return obj.e + f.mult(radiance(Ray(x, r.d - nl * 2 * nl.dot(r.d)), depth));
//		}
//		case REFR: {    // Ideal dielectric REFRACTION
//			Ray reflRay(x, r.d - n * 2 * n.dot(r.d));
//			bool into = n.dot(nl) > 0;                // Ray from outside going in?
//			double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
//			double ddn = r.d.dot(nl), cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
//
//			if (cos2t < 0)    // Total internal reflection
//				return obj.e + f.mult(radiance(reflRay, depth));    // only reflection term
//
//			Ray r_out(x, (r.d * nnt - nl * (ddn * nnt + sqrt(cos2t))).norm());
//			double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : r_out.d.dot(n));
//			double Re = R0 + (1 - R0) * pow(c, 5);
//			double Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P;
//			double TP = Tr / (1 - P);
//			return obj.e + f.mult(
//					depth > 2
//					? (randf() < P   // Russian roulette
//					   ? radiance(reflRay, depth) * RP
//					   : radiance(r_out, depth) * TP)
//					: radiance(reflRay, depth) * Re + radiance(r_out, depth) * Tr    // weighted addition
//			);
//		}
//		default:
//			return {};
////			throw "invalid refl_t value: " + std::to_string(obj.refl_t);
//	}
}
