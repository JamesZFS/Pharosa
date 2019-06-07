//
// Created by James on 2019/4/2.
//

#include "Algorithm.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include "../geometric/Sphere.h"
#include "../utils/sampling.h"

#include <omp.h>

#define OMP_ON 1

using Funcs::randf;

Algorithm::Algorithm(const Scene &scene_, Camera &camera_) : scene(scene_), camera(camera_)
{
}

Color Algorithm::getDirectLighting(const Intersection &isect) const
{
	auto &lights = scene.getLightSources();
	List<real> omegas, cos_theta_maxes;
	List<const Object *> sphere_lights;
	omegas.reserve(lights.size());
	cos_theta_maxes.reserve(lights.size());
	sphere_lights.reserve(lights.size());
	real S_omega = 0.f;	// sum
	for (auto light : lights) {
		if (light->geo->type() != Geometry::SPHERE)
			continue;    // deal only with sphere
		auto s = (Sphere *) light->geo;
		real dist = (s->c - isect.pos).norm();
		real sin_theta_max = s->rad / dist;
		if (sin_theta_max > 1)
			continue;    // unwanted sample
		real cos_theta_max = sqrtf(max2(0.f, 1 - sin_theta_max * sin_theta_max));
		real omega = 2 * M_PIF * (1 - cos_theta_max);

		sphere_lights.push_back(light);
		cos_theta_maxes.push_back(cos_theta_max);
		omegas.push_back(omega);
		S_omega += omega;
	}
	auto P = randf(0, S_omega);
	size_t i;	// selected light index
	for (i = 0; i < omegas.size(); ++i) {	// R.R.
		P -= omegas[i];
		if (P <= 0) break;
	}
	// sample
	auto s = (Sphere *) sphere_lights[i]->geo;
	Dir sz = s->c - isect.pos + isect.nl * EPS, sx, sy;
	sz.unitize();
	sz.getOrthogonalBasis(sx, sy);
	auto samp = Sampling::uniformOnSphereCap(cos_theta_maxes[i], {randf(), randf()});
	Ray r_out(isect.pos, sx * samp.x + sy * samp.y + sz * samp.z);
	if (isect.nl % r_out.dir <= 0)
		return Color::BLACK;	// shadow ray
	r_out.offset(EPS);
	Intersection nxt_isect;
	if (!scene.intersectAny(r_out, nxt_isect, true)) {    // some precision bug
		++__counter__;
		return Color::BLACK;
	}
	if (nxt_isect.hit != lights[i])
		return Color::BLACK;    // shadow ray

	/**	Ld = f * Le_i * cos(theta_i) / pdf(omega_i)
	 * where f = 1 / pi, and pdf = 1 / Sum(omega_i, i)
	 */
	return lights[i]->mtr->emi * (isect.nl % r_out.dir * M_1_PI * S_omega);
}

void Algorithm::start(size_t n_epoch,
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
				camera.draw(i, j, radiance(ray));
			}
		}
		camera.step();
		post_epoch_callback(epoch + 1);
	}
}
