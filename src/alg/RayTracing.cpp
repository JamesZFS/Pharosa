//
// Created by James on 2019/4/2.
//

#include "../utils/funcs.hpp"
#include "RayTracing.h"

RayTracing::RayTracing(const Stage &stage_, Cameras::Camera &camera_) :
		GI(stage_, camera_)
{
}

Color RayTracing::radiance(const Ray &ray, unsigned int depth) const
{
	using namespace Scenes;
	using Funcs::randf;

	// calculate intersection
	Pos x;    // hitting point
	Dir normal;
	const Object *hit = nullptr;
	if (!stage.intersectAny(ray, hit, x, normal)) return Color::BLACK; // if miss, return black
	assert(hit != nullptr);    // todo

	const Object &obj = *hit;    // the hit object
	Dir nl = normal % ray.dir < 0 ? normal : normal * -1;    // regularized normal, against in direction
	Color color = obj.color;    // color to render
	double p = color.max();    // max color component as refl_t

	// stop tracing when depth too large or color too dim
	if (++depth > 5 || p < EPS) {    // depth limit
		// todo
//		if (randf() < p) color /= p;    // brighter
//		else return obj.emi; // R.R. the darker the obj is, the more likely to stop radiating
		return obj.emi; // R.R. the darker the obj is, the more likely to stop radiating
	}

	// recursively trace back
	// todo implement BRDF / BTDF which receives an incidence and returns a reflection
	// todo light attenuation
	switch (obj.reft) {

		// Ideal diffusive reflection
		case Object::DIFF: {
			double r1 = 2 * M_PI * randf(), r2 = randf(), r2s = sqrt(r2);
			Dir ex, ey;
			const Dir &ez = nl;
			ez.getOrthogonalBasis(ex, ey);

			Dir d = ex * cos(r1) * r2s + ey * sin(r1) * r2s + ez * sqrt(1 - r2);
			return obj.emi + color.mul(radiance(Ray(x, d), depth));
		}

		// Ideal mirror reflection
		case Object::REFL: {
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

void RayTracing::render(unsigned int n_epoch, unsigned int prev_epoch,
						unsigned int verbose_step, const String &checkpoint_dir)
{
	if (verbose_step > 0) {
		renderVerbose(n_epoch, prev_epoch, verbose_step, checkpoint_dir);    // with progressbar
		return;
	}
	// without progressbar
	Buffer buffer;
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;
	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; ++epoch) {
		debug("\n=== epoch %d / %d ===\n", epoch + 1, tot_epoch);
		camera.resetProgress();
		while (!camera.finished()) {
			const Ray &ray = camera.shootRay();
			auto color = radiance(ray, 0);
			camera.renderInc(color);
			camera.updateProgress();
		}
		if (checkpoint) {    // save checkpoints
			sprintf(buffer, "%s/epoch - %d.ppm", checkpoint_dir.data(), epoch);
			camera.writePPM(buffer);
		}// todo update code
	}
}

void RayTracing::renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
							   unsigned int verbose_step, const String &checkpoint_dir)
{
	// without progressbar
	Buffer buffer;
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;

	// todo use openmp to boost, (single camera, multi pixel to render simultaneously )

	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; ++epoch) {
		debug("\n=== epoch %d / %d ===\n", epoch + 1, tot_epoch);
		fflush(stdout);
		camera.resetProgress();
		while (!camera.finishedVerbose(verbose_step)) {    // slight difference here
			const Ray &ray = camera.shootRay();
			for (unsigned int k = 0; k < 4; ++k) {    // repeat 4 times
				camera.renderInc(radiance(ray, 0));
			}
			camera.updateProgress();
		}
		if (checkpoint) {    // save checkpoints
			sprintf(buffer, "%sepoch - %d.ppm", checkpoint_dir.data(), epoch);
			camera.writePPM(buffer);
		}
	}
	debug("\n");
}
