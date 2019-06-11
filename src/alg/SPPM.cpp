//
// Created by James on 2019/6/7.
//

#include "SPPM.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../geometric/Sphere.h"
#include "../utils/sampling.h"

#define OMP_ON 1

using Funcs::randf;


SPPM::SPPM(const Scene &scene_, Camera &camera_,
		   size_t n_photon_per_iter_, size_t max_depth_, real init_radius_) :
		Algorithm(scene_, camera_),
		n_photon_per_iter(n_photon_per_iter_), max_depth(max_depth_), init_radius(init_radius_),
		kd_root(nullptr)
{
}

SPPM::~SPPM()
{
	delete kd_root;
}

String SPPM::info() const
{
	Buffer buffer;
	sprintf(buffer,
			"<Stochastic Progressive Photon Mapping> n_photon_per_iter = %ld, max_depth = %ld, init_radius = %f",
			n_photon_per_iter, max_depth, init_radius);
	return buffer;
}

void SPPM::start(size_t n_epoch, size_t save_step,
				 const std::function<void(size_t)> &pre_epoch_callback,
				 const std::function<void(size_t)> &in_epoch_callback,
				 const std::function<void(size_t)> &post_epoch_callback)
{
	// init vps
	visible_points.assign(camera.width, List<VisiblePoint>(camera.height));    // width x height
	for (size_t j = 0; j < camera.height; ++j) {
		for (size_t i = 0; i < camera.width; ++i) {
			visible_points[i][j].r = init_radius;
		}
	}
	real r_bound = init_radius;    // upper bound of r
	auto &lights = scene.getLightSources();
	for (auto lt : lights) {
		assert(lt->geo->type() == Geometry::SPHERE);
	}
	camera.step();
	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {
		pre_epoch_callback(epoch);

		// pass 1:
		// let camera shoot rays
		// for each shot ray, trace its path in the scene to form visible points
		// build kd tree to hold all visible points

		printf("\033[32m[ New Epoch, r_bound = %.4f ]\033[0m\n", r_bound);
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

		message("\nNow building KD tree...\n");
		buildKDTree();

		// pass 2:
		// for each photon in n_photon_per_iteration
		// randomly sample one light from source
		// trace it in the scene and query kd tree for path's nearby visible points
		// contribute photon radiance to kd node

		message("Now processing photon tracing...\n");
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t i = 0; i < n_photon_per_iter; ++i) {
			in_epoch_callback(i * camera.height / n_photon_per_iter);
			// randomly pick one light source
			auto lt = lights.at((size_t) randf(0, lights.size()));
			auto &s = *(Sphere *) lt->geo;
//		real wl = s->area() / area_sum;
			real pdf = 1.f / lights.size();
			// randomly pick one pos on light source
			auto samp = Sampling::uniformOnSphere({randf(), randf()});
			Pos pos = samp * s.rad;
			Dir ex, ey, ez = samp - s.c;
			ez.getOrthogonalBasis(ex, ey);
			samp = Sampling::uniformOnHemiSphere({randf(), randf()});
			pdf *= 1.f / (s.area() * 2 * M_PIF);    // todo
//			pdf *= 1.f / (4 * M_PIF * 2 * M_PIF);
			Dir dir = ex * samp.x + ey * samp.y + ez * samp.z;
			Ray ri(pos, dir);

			// trace light
			tracePhoton(ri, lt->mtr->emi / pdf, r_bound);
		}
//		drawCurPhotons(epoch);

		// pass over:
		// update pixel radiance values according to kd node info
		const static real gamma = 2.f / 3.f;
		message("Now updating params...\n");

		r_bound = 0;    // update upper bound of r
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t j = 0; j < camera.height; ++j) {
			in_epoch_callback(j);
			for (size_t i = 0; i < camera.width; ++i) {
				auto &vp = visible_points[i][j];
				real N_new = vp.N + gamma * vp.M;
				real r_new = N_new > 0 ? vp.r * sqrtf(N_new / (vp.N + vp.M)) : vp.r;
				Color tau_new = (vp.tau + vp.beta.mul(vp.Phi)) * ((r_new * r_new) / (vp.r * vp.r));

				vp.N = N_new;
				vp.r = r_new;
				vp.tau = tau_new;

				vp.M = 0;
				vp.Phi = {0, 0, 0};
				vp.beta = {0, 0, 0};
				r_bound = max2(r_bound, vp.r);    // update upper bound of r
			}
		}
//		drawLi(epoch);

		if (save_step > 0 && (epoch + 1) % save_step == 0) {    // periodically save image
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
			for (size_t j = 0; j < camera.height; ++j) {
				for (size_t i = 0; i < camera.width; ++i) {
					auto &vp = visible_points[i][j];
					camera.draw(i, j,
								vp.Ld / (epoch + 1) +
								vp.tau / ((epoch + 1) * n_photon_per_iter * M_PIF * vp.r * vp.r));    // Li
				}
			}
			post_epoch_callback(epoch + 1);
		}
	}    // next epoch

	// generate image
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
			vp.pos = isect.pos;
			vp.Ld += vp.beta.mul(LdFaster(isect));
			vp.wo = -ro.dir;
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
//	kd_root = new NaiveGrid(vps);
	kd_root = new KDGrid(vps);
}

// trace it in the scene and query kd tree for path's nearby visible points
// contribute photon radiance to kd node
void SPPM::tracePhoton(Ray ri, Color beta, real r_bound)
{
	for (size_t depth = 0; depth < max_depth; ++depth) {
		Intersection isect;
		if (!scene.intersectAny(ri, isect)) break;
		Ray r_new;
		real w_new;
		auto type = isect.scatter(ri, r_new, w_new);
		beta *= std::abs(ri.dir % isect.nl);    // v1 absorb ratio
		VPPtrList vps;
		vps.reserve(200);
		if (type == Intersection::DIFFUSE) {    // v2?
			vps.clear();
			// contribute to all visible points in the vicinity
			if (depth > 0) {    // skip direct lighting term
				kd_root->query(isect.pos, r_bound, [&isect, &beta](VisiblePoint *vp) {	// callback
					if (vp->wo % isect.nl < 0) return;    // in different surfaces
					++vp->M;
					// Phi += beta_j * f(wo, p, wj) where f = 1/pi * (wj * nl)
					vp->Phi += beta * M_1_PIF;
				});
			}
		}
		// trace next depth
		Color beta_new = beta / w_new; // todo
		beta_new *= isect.getColor();    // v1
//		beta_new *= isect.getColor() * M_1_PIF;  // v2

		// Possibly terminate photon path with Russian Roulette
		real P = min2(1.f, beta_new.max() / beta.max());
		if WITH_PROB(P)
			beta = beta_new / P;
		else
			break;

		// into next iter
		ri = r_new;
		beta = beta_new;
	}
}

void SPPM::drawCurPhotons(size_t epoch) const
{
	// draw debug photon info
	Image img(camera.width, camera.height);
	real Phi_max = 0;
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < img.getHeight(); ++j) {
		for (size_t i = 0; i < img.getWidth(); ++i) {
			auto &vp = visible_points[i][j];
			Phi_max = max2(Phi_max, vp.Phi.norm());
			img.at(i, j) = vp.Phi;
		}
	}
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < img.getHeight(); ++j) {
		for (size_t i = 0; i < img.getWidth(); ++i) {
			img.at(i, j) /= Phi_max;
		}
	}
	Buffer title;
	sprintf(title, "out/photons - %ld.ppm", epoch);
	img.writePPM(title, 1);
}

void SPPM::drawLi(size_t epoch) const
{
	Image img(camera.width, camera.height);
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < img.getHeight(); ++j) {
		for (size_t i = 0; i < img.getWidth(); ++i) {
			auto &vp = visible_points[i][j];
			Color Li = vp.tau / ((epoch + 1) * n_photon_per_iter * M_PIF * vp.r * vp.r);
			img.at(i, j) = Li;
		}
	}
	Buffer title;
	sprintf(title, "out/Li - %ld.ppm", epoch);
	img.writePPM(title, 1);
}
