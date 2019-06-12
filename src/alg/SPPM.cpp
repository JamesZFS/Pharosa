//
// Created by James on 2019/6/7.
//

#include "SPPM.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../geometric/Sphere.h"
#include "../geometric/Triangle.h"
#include "../utils/sampling.h"

using Funcs::randf;


SPPM::SPPM(const Scene &scene_, Camera &camera_,
		   size_t n_photon_per_iter_, size_t max_depth_, real init_radius_) :
		Algorithm(scene_, camera_),
		n_photon_per_iter(n_photon_per_iter_), max_depth(max_depth_), init_radius(init_radius_),
		grids(nullptr), lights(scene_.getLightSources())
{
}

SPPM::~SPPM()
{
	delete grids;
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
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < camera.height; ++j) {
		for (size_t i = 0; i < camera.width; ++i) {
			visible_points[i][j].r = init_radius;
		}
	}
	camera.step();    // one step always
	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {
		pre_epoch_callback(epoch);
		real r_avg = 0;    // average search radius
		for (size_t j = 0; j < camera.height; ++j) {
			for (size_t i = 0; i < camera.width; ++i) {
				r_avg += visible_points[i][j].r;
			}
		}
		r_avg /= camera.height * camera.width;
		// pass 1:
		// let camera shoot rays
		// for each shot ray, trace its path in the scene to form visible points
		// build grid data structure to hold all visible points

		printf("\033[32m[ New Epoch, r_avg = %.4f ]\033[0m\n", r_avg);
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

		message("\nNow building grid...\n");
		buildGrid();

		// pass 2:
		// for each photon in n_photon_per_iteration
		// randomly sample one light from source
		// trace it in the scene and query grid for path's nearby visible points
		// contribute photon radiance to grid cell

		message("Now processing photon tracing...\n");
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t i = 0; i < n_photon_per_iter; ++i) {
			in_epoch_callback(i * camera.height / n_photon_per_iter);
			// randomly pick one light source
			Color beta;
			Ray ri = sampleOneLight(beta);

			// trace light path
			tracePhoton(ri, beta, r_avg);
		}
#if DEBUG_DRAW_ON
		drawPhi(epoch);
		drawM(epoch);
#endif

		// pass over:
		// update pixel radiance values according to grid cell's info
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
				real r_new = N_new > 0 ? vp.r * sqrtf(N_new / (vp.N + vp.M)) : vp.r;
				Color tau_new = (vp.tau + vp.beta.mul(vp.Phi)) * ((r_new * r_new) / (vp.r * vp.r));

				vp.N = N_new;
				vp.r = r_new;
				vp.tau = tau_new;

				vp.M = 0;
				vp.Phi = {0, 0, 0};
				vp.beta = {0, 0, 0};
			}
		}
#if DEBUG_DRAW_ON
		drawLi(epoch);
#endif

		if (save_step > 0 && (epoch + 1) % save_step == 0) {    // periodically save image
			capture(epoch + 1);
			post_epoch_callback(epoch + 1);
		}
	}    // next epoch
	capture(n_epoch);
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
#if LD_ON
			vp.Ld += vp.beta.mul(computeLd(isect));
#endif
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

void SPPM::buildGrid()
{
	delete grids;
	VPPtrList vps;    // flattened
	vps.reserve(camera.height * camera.width);
	for (auto &col : visible_points) {
		for (auto &vp : col) {
			vps.push_back(&vp);
		}
	}
	grids = new UniformGrid(vps);
}

// trace it in the scene and query grid for path's nearby visible points
// contribute photon radiance to grid cell
void SPPM::tracePhoton(Ray ri, Color beta, real r_bound)
{
	for (size_t depth = 0; depth < max_depth; ++depth) {
		Intersection isect;
		if (!scene.intersectAny(ri, isect)) break;
		Ray r_new;
		real w_new;
		auto type = isect.scatter(ri, r_new, w_new);
		beta *= std::abs(ri.dir % isect.nl);    // v1 absorb ratio
		if (type == Intersection::DIFFUSE) {    // v2?
			// contribute to all visible points in the vicinity
#if LD_ON
			if (depth > 0) {    // skip direct lighting term
#else
			{
#endif
				grids->query(isect.pos, r_bound, [&isect, &beta](VisiblePoint *vp) {    // callback
					if (vp->wo % isect.nl <= 0) return;    // in different surfaces
					++vp->M;
					// Phi += beta_j * f(wo, p, wj) where f = 1/pi * (wj * nl)
					vp->Phi += beta * M_1_PIF;
				});
			}
		}
		// trace next depth
		Color beta_new = beta * w_new;
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

Ray SPPM::sampleOneLight(Color &beta)
{
	Ray ri;
	// randomly choose one light
	auto lt = lights.at((size_t) randf(0, lights.size()));
	real inv_pdf = lights.size();    // 1/pdf
	switch (lt->geo->type()) {
		case Geometry::SPHERE: {
			auto &s = *(Sphere *) lt->geo;
			// randomly pick one pos on light source
			auto samp = Sampling::uniformOnSphere({randf(), randf()});
			inv_pdf *= s.area();
			ri.org = s.c + samp * s.rad;
			Dir ex, ey, ez = samp;
			ez.getOrthogonalBasis(ex, ey);
			samp = Sampling::uniformOnHemisphere({randf(), randf()});
			inv_pdf *= 2 * M_PIF; // todo
			ri.dir = ex * samp.x + ey * samp.y + ez * samp.z;
			break;
		}
		case Geometry::TRIANGLE: {
			auto &t = *(Triangle *) lt->geo;
			ri.org = Sampling::uniformOnTriangle(t, {randf(), randf()});
			inv_pdf *= t.area();
			// randomly pick one side to shoot
			Dir ex, ey, ez = WITH_PROB(0.5) ? t.n : -t.n;
			ez.getOrthogonalBasis(ex, ey);
			auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
			inv_pdf *= 2 * M_PIF;
			ri.dir = ex * samp.x + ey * samp.y + ez * samp.z;
			break;
		}
		default: TERMINATE("Got unsupported finite shape light source for sampling!")
	}
	beta = lt->mtr->emi * inv_pdf;
	ri.offset(EPS);
	return ri;
}

// debuggers:
void SPPM::drawPhi(size_t epoch) const
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
	sprintf(title, "out/Phi - %ld.ppm", epoch);
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

void SPPM::drawM(size_t epoch) const
{
// draw debug photon info
	Image img(camera.width, camera.height);
	real M_max = 0;
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < img.getHeight(); ++j) {
		for (size_t i = 0; i < img.getWidth(); ++i) {
			auto &vp = visible_points[i][j];
			M_max = max2((size_t) M_max, vp.M);
			img.at(i, j) = vp.M;
		}
	}
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (size_t j = 0; j < img.getHeight(); ++j) {
		for (size_t i = 0; i < img.getWidth(); ++i) {
			img.at(i, j) /= M_max;
		}
	}
	Buffer title;
	sprintf(title, "out/M - %ld.ppm", epoch);
	img.writePPM(title, 1);
}

void SPPM::capture(size_t n_epoch)
{
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
