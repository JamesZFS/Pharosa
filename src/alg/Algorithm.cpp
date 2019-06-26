//
// Created by James on 2019/4/2.
//

#include "Algorithm.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include "../geometric/Sphere.h"
#include "../utils/sampling.h"
#include "../geometric/Triangle.h"

#include <omp.h>

using Funcs::randf;

Algorithm::Algorithm(const Scene &scene_, Camera &camera_) : scene(scene_), camera(camera_)
{
}

Color Algorithm::computeLd(const Intersection &isect) const
{
	auto &lights = scene.getLightSources();    // all finite lights
	// randomly select one light source
	size_t N = lights.size();
	auto light = lights.at((size_t) randf(0, N));
	Ray r_out;
	r_out.org = isect.pos;
	real inv_pdf = N;    // 1/pdf
	real G;
	// we just have to make sure the sampling strategy matches the pdf
	switch (light->geo->type()) {
		case Geometry::SPHERE: {
			auto &s = *(Sphere *) light->geo;
			Pos OS = s.c - isect.pos + isect.nl * EPS;
			real dist = OS.norm();
			Dir sz = OS / dist, sx, sy;
			sz.getOrthogonalBasis(sx, sy);
			real sin_theta_max = s.rad / dist;
//			if (sin_theta_max > 1)
//				return Color::BLACK;    // unwanted sample
			real cos_theta_max = sqrtf(max2(0.f, 1 - sin_theta_max * sin_theta_max));
			auto samp = Sampling::uniformOnSphereCap(cos_theta_max, {randf(), randf()});
			r_out.dir = sx * samp.x + sy * samp.y + sz * samp.z;
			G = isect.nl % r_out.dir;
			if (G <= 0)
				return Color::BLACK;    // shadow ray
			inv_pdf *= 2 * M_PIF * (1 - cos_theta_max);
			break;
		}
		case Geometry::TRIANGLE: {
			auto &t = *(Triangle *) light->geo;
			Pos d = Sampling::uniformOnTriangle(t, {randf(), randf()}) - isect.pos;
			real dist = d.norm();
			r_out.dir = d / dist;
			G = isect.nl % r_out.dir;
			if (G <= 0)
				return Color::BLACK;    // shadow ray
			G *= fabsf(r_out.dir % t.n) / (dist * dist);
			inv_pdf *= t.area();
			break;
		}
		default: TERMINATE("Got unsupported finite shape light source!")
	}
	r_out.offset(EPS);
	Intersection nxt_isect;
	if (!scene.intersectAny(r_out, nxt_isect, false))    // a missing sample todo
		return Color::BLACK;
	if (nxt_isect.hit != light)
		return Color::BLACK;    // shadow ray

	/**	Ld = f * Le_i * G / pdf
 	 * where f = 1 / pi, G = cos(theta_i) for sphere or cos(theta_i) * cos(theta_i') / dist^2
 	 */
//	return light->mtr->emi * (M_1_PIF * G * inv_pdf);
	return nxt_isect.getEmission() * (M_1_PIF * G * inv_pdf);
}

void Algorithm::start(size_t n_epoch, size_t save_step,
					  const std::function<void(size_t)> &pre_epoch_callback,
					  const std::function<void(size_t)> &in_epoch_callback,
					  const std::function<void(size_t)> &post_epoch_callback)
{
	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {    // for samples
		pre_epoch_callback(epoch);
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t j = 0; j < camera.height; ++j) {                // for each pixel todo
			in_epoch_callback(j);
			for (size_t i = 0; i < camera.width; ++i) {
				auto ray = camera.shootRayAt(i, j, 0.5);    // rand normal AA
				camera.drawInc(i, j, radiance(ray));
			}
		}
		camera.step();
		if (save_step > 0 && (epoch + 1) % save_step == 0)    // periodically save
			post_epoch_callback(epoch + 1);
	}
}
