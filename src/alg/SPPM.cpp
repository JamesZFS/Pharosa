//
// Created by James on 2019/6/7.
//

#include "SPPM.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../geometric/Sphere.h"
#include "../utils/sampling.h"

using Funcs::randf;

SPPM::SPPM(const Scene &scene_, Camera &camera_,
							 size_t n_photon_per_iter_, size_t max_depth_) :
		Algorithm(scene_, camera_),
		n_photon_per_iter(n_photon_per_iter_), max_depth(max_depth_)
{

}

SPPM::~SPPM()
{
	delete kd_root;
}

String SPPM::info() const
{
	return "<Stochastic Progressive Photon Mapping>";
}

void SPPM::start(size_t n_epoch,
						  const std::function<void(size_t)> &pre_epoch_callback,
						  const std::function<void(size_t)> &in_epoch_callback,
						  const std::function<void(size_t)> &post_epoch_callback)
{

	// init vps
	visible_points.assign(camera.width, List<VisiblePoint>(camera.height));    // width x height
	assert(visible_points.size() == camera.width);
	assert(visible_points[0].size() == camera.height);

	real r_bound = 1.f;    // upper bound of r todo

	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {
		pre_epoch_callback(epoch);

		// pass 1:
		// let camera shoot rays
		// for each shot ray, trace its path in the scene to form visible points
		// build kd tree to hold all visible points

		message("Now tracing visible points...\n");
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t j = 0; j < camera.height; ++j) {
			in_epoch_callback(j);
			for (size_t i = 0; i < camera.width; ++i) {
				Ray ro = camera.shootRayAt(i, j, 0.5);
				traceCameraRay(ro, visible_points[i][j]);
			}
		}

		message("\nNoew building KD tree...\n");
		buildKDTree();

		// pass 2:
		// for each photon in n_photon_per_iteration
		// randomly sample one light from source
		// trace it in the scene and query kd tree for path's nearby visible points
		// contribute photon radiance to kd node

		auto &lights = scene.getLightSources();
//	real area_sum = 0;
		for (auto lt : lights) {
			assert(lt->geo->type() == Geometry::SPHERE);
//		auto s = (Sphere *) lt->geo;
//		area_sum += s->area();
		}

		message("Now processing photon tracing...\n");
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t i = 0; i < n_photon_per_iter; ++i) {
			// randomly select one light source
			auto lt = lights.at((size_t) randf(0, lights.size()));
			auto &s = *(Sphere *) lt->geo;
//		real wl = s->area() / area_sum;
			real pdf = 1.f / lights.size();
			auto samp = Sampling::uniformOnSphere({randf(), randf()});
			Pos pos = samp * s.rad;
			Dir ex, ey, ez = samp - s.c;
			ez.getOrthogonalBasis(ex, ey);
			samp = Sampling::uniformOnHemiSphere({randf(), randf()});
			pdf *= 1.f / (s.area() * 2 * M_PIF);
			Dir dir = ex * samp.x + ey * samp.y + ez * samp.z;
			Ray ri(pos, dir);

			// trace light
			tracePhoton(ri, lt->mtr->emi / pdf, r_bound);
			// todo update r
		}

		// pass over:
		// update pixel radiance values according to kd node info
		const static real gamma = 2.f / 3.f;
		message("Now updating params...\n");

#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t j = 0; j < camera.height; ++j) {
			in_epoch_callback(j);
			for (size_t i = 0; i < camera.width; ++i) {
				auto &vp = visible_points[i][j];
				real N_new = vp.N + gamma * vp.M;
				real r_new = vp.r * sqrtf(N_new / (vp.N + vp.M));
				Color tau_new = (vp.tau + vp.Phi) * ((r_new * r_new) / (vp.r * vp.r));

				vp.N = N_new;
				vp.r = r_new;
				vp.tau = tau_new;

				vp.M = 0;
				vp.Phi = {0, 0, 0};
				vp.beta = {0, 0, 0};
			}
		}
		post_epoch_callback(epoch + 1);
	}
	// draw image
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < camera.height; ++j) {
		for (size_t i = 0; i < camera.width; ++i) {
			auto &vp = visible_points[i][j];
			Color L = vp.Ld / n_epoch +
					  vp.tau / (n_epoch * n_photon_per_iter * M_PIF * vp.r * vp.r);
			camera.draw(i, j, L);
		}
	}
	camera.step();
}

void SPPM::traceCameraRay(Ray ro, VisiblePoint &vp)
{
	vp.beta = {1, 1, 1};    // total throughput
	bool flag = false;

	for (size_t depth = 0; depth < max_depth; ++depth) {
		// calculate intersection:
		Intersection isect;
		if (!scene.intersectAny(ro, isect)) break; // if miss, stop tracing

		// accumulate radiance and throughput
		vp.Ld += vp.beta.mul(isect.getEmission());
		vp.beta *= isect.getColor();

		// update incoming ray origin
		real w_out;
		Ray ri;
		// sample one new ray
		auto scatter_type = isect.scatter(ro, ri, w_out);

		// account for direct lighting and break loop if diffuse
		if (scatter_type == Intersection::DIFFUSE) {
			flag = true;
			vp.Ld += vp.beta.mul(LdFaster(isect));
			break;
		}

		ro = ri;
		vp.beta *= w_out;
		// next depth
	}
	if (!flag) {    // not hit any diff surface
		++__counter__;
		vp.beta = Color::BLACK;
	}
}

void SPPM::buildKDTree()
{
	delete kd_root;
	VPPtrList vps;
	for (auto &col : visible_points) {
		for (auto &vp : col) {
			vps.push_back(&vp);
		}
	}
	kd_root = new KDGrid(vps);
}

// trace it in the scene and query kd tree for path's nearby visible points
// contribute photon radiance to kd node
void SPPM::tracePhoton(Ray ri, Color beta, real r_bound)
{
	for (size_t depth = 0; depth < max_depth; ++depth) {
		Intersection isect;
		if (!scene.intersectAny(ri, isect)) break;
		// contribute to all visible points in the vicinity
		VPPtrList vps;
		if (depth > 0) {    // skip direct lighting term
			bool found_vp = kd_root->query(isect.pos, r_bound, vps);
			if (found_vp) {
				for (auto vp_ptr : vps) {
					auto &vp = *vp_ptr;
					assert(!vp.beta.isBlack());
					++vp.M;
					// Phi += beta_j * f(wo, p, wj), but here f === 1/pi
					vp.Phi += beta * M_1_PIF;
				}
			}
		}
		// trace next depth
		Ray r_new;
		real w_new;
		isect.scatter(ri, r_new, w_new);
		Color beta_new = beta * w_new;
		// Possibly terminate photon path with Russian roulette
		real P = beta_new.max() / beta.max();
		assert(0 < P && P <= 1);
		if WITH_PROB(P)
			beta = beta_new / P;
		else
			break;

		// into next iter
		ri = r_new;
		beta = beta_new;
	}
}
