//
// Created by James on 2019/4/2.
//

#include "RayTracing.h"

RayTracing::RayTracing(const Stage &stage_, Cameras::Camera &camera_) :
		GI(stage_, camera_), distribution(0.0, 1.0)
{
	generator.seed((unsigned int) time(nullptr));
}

double RayTracing::randf()
{
	return distribution(generator);
}

Color RayTracing::radiance(const Ray &ray, unsigned int depth)
{
	using namespace Scenes;
	// calculate intersection
	double t;        // distance to intersection
	const Object *hit = nullptr;
	if (!stage.intersectAny(ray, t, hit)) return Color::BLACK; // if miss, return black
	assert(hit != nullptr);    // todo
	const auto &obj = *hit;        // the hit object

	// forward ray
	Pos x = ray.org + ray.dir * t;           // hitting point
	Pos n = (x - obj.pos).unitize();            // unitizeal
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
			Pos i = ((fabs(k.x) > .1 ? Pos(0, 1) : Pos(1)) ^ k).unitize();    // .1 is the max threshold value for k.x
			Pos j = k ^i;    // i, j, k(nl) coordinate system
			Pos d = (i * cos(r1) * r2s + j * sin(r1) * r2s + k * sqrt(1 - r2)).unitize();
			return obj.emi + f.mul(radiance(Ray(x, d), depth));
		}
		case Object::REFL: {         // Ideal mirror reflection
			return obj.emi + f.mul(radiance(Ray(x, ray.dir - nl * 2 * (nl % ray.dir)), depth));
		}
		case Object::REFR: {    // Ideal dielectric refraction
			Ray reflRay(x, ray.dir - n * 2 * (n % ray.dir));
			bool into = (n % nl) > 0;                // Ray from outside going in?
			double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
			double ddn = ray.dir % nl, cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			if (cos2t < 0)    // Total internal reflection
				return obj.emi + f.mul(radiance(reflRay, depth));    // only reflection term

			Ray r_out(x, (ray.dir * nnt - nl * (ddn * nnt + sqrt(cos2t))).unitize());
			double a = nt - nc, b = nt + nc, c = 1 - (into ? -ddn : r_out.dir % n);
			double R0 = a * a / (b * b), Re = R0 + (1 - R0) * pow(c, 5);
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
		}// todo
	}
}

void RayTracing::renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
							   unsigned int verbose_step, const String &checkpoint_dir)
{
	// without progressbar
	Buffer buffer;
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;
	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; ++epoch) {
		debug("\n=== epoch %d / %d ===\n", epoch + 1, tot_epoch);
		fflush(stdout);
		camera.resetProgress();
		while (!camera.finishedVerbose(verbose_step)) {    // slight difference here
			const Ray &ray = camera.shootRay();
			for (unsigned int k = 0; k < 4; ++k) {	// repeat 4 times
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
