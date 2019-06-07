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
	Color Ld = {0, 0, 0};
	for (const Object *ls : scene.getLightSources()) {
		if (ls->geo->type() != Geometry::SPHERE)
			continue;    // deal only with sphere
		auto s = (Sphere *) ls->geo;
		Pos OS = s->c - isect.pos + isect.nl * EPS;
		real dist = OS.norm();
		Dir sz = OS / dist, sx, sy;
		sz.getOrthogonalBasis(sx, sy);
		real sin_theta_max = s->rad / dist;
		if (sin_theta_max > 1)
			continue;    // unwanted sample
		real cos_theta_max = sqrtf(max2(0.f, 1 - sin_theta_max * sin_theta_max));
		auto samp = Sampling::uniformOnSphereCap(cos_theta_max, {randf(), randf()});
		Ray r_out(isect.pos, sx * samp.x + sy * samp.y + sz * samp.z);
		if (isect.nl % r_out.dir <= 0)
			continue;	// shadow ray
		r_out.offset(EPS);
		Intersection nxt_isect;
		if (!scene.intersectAny(r_out, nxt_isect, true)) {    // some precision bug
			++__counter__;
			continue;
		}
		if (nxt_isect.hit != ls)
			continue;    // shadow ray
		Ld += ls->mtr->emi * ((isect.nl % r_out.dir) * 2 * (1 - cos_theta_max));
		// notice: uniform sampling, so divided by Pi already
	}
	return Ld;
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
