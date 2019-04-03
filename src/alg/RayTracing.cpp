//
// Created by James on 2019/4/2.
//

#include "RayTracing.h"

RayTracing::RayTracing(const Stage &stage_, Cameras::BasicCamera &camera_, unsigned int random_seed) :
		GI(stage_, camera_), distribution(0.0, 1.0)
{
	generator.seed(random_seed);
}

double RayTracing::randf()
{
	return distribution(generator);
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

Color RayTracing::radiance(const Ray &ray, unsigned int depth)
{
	using namespace Scenes;
	double t;                               // distance to intersection
	int id = 0;                               // id of intersected object
	Object *hit = nullptr;
	// calculate intersection
	if (!stage.intersectAny(ray, t, hit)) return BLACK; // if miss, return black
	assert(hit != nullptr);    // todo
	const auto &obj = *hit;        // the hit object

	// forward ray
	Pos x = ray.org + ray.dir * t;                    // hitting point
	Pos n = (x - obj.pos).norm();                // normal
	Pos nl = n % ray.dir < 0 ? n : n * -1;
	Pos f = obj.color;
	double p = f.max(); // max color component as refl_t

	// stop tracing when depth too large
	if (++depth > 5 || p < EPS) {    // depth limit
		if (randf() < p) f = f * (1 / p);    // brighter?
		else return obj.emi; //R.R.	the darker, the more likely to stop radiating
	}

	// recursively trace back
	switch (obj.reft) {
		case Object::DIFF: {           // Ideal diffusive reflection
			double r1 = 2 * M_PI * randf(), r2 = randf(), r2s = sqrt(r2);
			Pos k = nl;
			Pos i = ((fabs(k.x) > .1 ? Pos(0, 1) : Pos(1)) ^ k).norm();    // .1 is the max threshold value for k.x
			Pos j = k ^i;    // i, j, k(nl) coordinate system
			Pos d = (i * cos(r1) * r2s + j * sin(r1) * r2s + k * sqrt(1 - r2)).norm();
			return obj.emi + f.mul(radiance(Ray(x, d), depth));
		}
		case Object::MIRR: {         // Ideal mirror reflection
			return obj.emi + f.mul(radiance(Ray(x, ray.dir - nl * 2 * (nl % ray.dir)), depth));
		}
		case Object::REFR: {    // Ideal dielectric refraction
			Ray reflRay(x, ray.dir - n * 2 * (n % ray.dir));
			bool into = (n % nl) > 0;                // Ray from outside going in?
			double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
			double ddn = ray.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			if (cos2t < 0)    // Total internal reflection
				return obj.emi + f.mul(radiance(reflRay, depth));    // only reflection term

			Ray r_out(x, (ray.dir * nnt - nl * (ddn * nnt + sqrt(cos2t))).norm());
			double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : r_out.dir % n);
			double Re = R0 + (1 - R0) * pow(c, 5);
			double Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P;
			double TP = Tr / (1 - P);
			return obj.emi + f.mul(
					depth > 2
					? (randf() < P   // Russian roulette
					   ? radiance(reflRay, depth) * RP
					   : radiance(r_out, depth) * TP)
					: radiance(reflRay, depth) * Re + radiance(r_out, depth) * Tr    // weighted addition
			);
		}
		default:
			char buffer[50];
			sprintf(buffer, "Warning: got invalid reft value \"%d\", rendering as black.\n", obj.reft);
			warn(buffer);
			return BLACK;
	}
}
