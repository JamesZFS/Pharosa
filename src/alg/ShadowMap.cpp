//
// Created by James on 2020/2/23.
//

#include "ShadowMap.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include "../utils/sampling.h"

using Funcs::randf;
using Funcs::clamp;

ShadowMap::ShadowMap(const Scene &scene, Camera &camera) : Algorithm(scene, camera) {}

String ShadowMap::info() const {
    return "<Shadow Map>";
}

void ShadowMap::start(size_t n_epoch, size_t save_step, const std::function<void(size_t)> &pre_epoch_callback,
                      const std::function<void(size_t)> &in_epoch_callback,
                      const std::function<void(size_t)> &post_epoch_callback) {
    Wlv_buffer.resize(camera.width, List<real>(camera.height));
    Wl_buffer.resize(camera.width, List<real>(camera.height));
    camera.step();
    assert(camera.drawCount() == 1);
    // initialize
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
    for (size_t j = 0; j < camera.height; ++j) {
        for (size_t i = 0; i < camera.width; ++i) {
            Wlv_buffer[i][j] = Wl_buffer[i][j] = 0;
        }
    }

    // main render loop
    for (size_t samp = 0; samp < n_epoch; ++samp) {
        pre_epoch_callback(samp);
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
        for (size_t j = 0; j < camera.height; ++j) {                // for each pixel
            Intersection first_isect;
            real Wlv, Wl;
            in_epoch_callback(j);
            for (size_t i = 0; i < camera.width; ++i) {
                auto ray = camera.shootRayAt(i, j, 0.5);    // rand normal AA
                if (scene.intersectAny(ray, first_isect)) {
                    estimateShadow(first_isect, Wlv, Wl);
                }
                else { // miss, return black
                    Wlv = 0;
                    Wl = 1;
                }
                Wlv_buffer[i][j] = Wlv, Wl_buffer[i][j] = Wl;
            }
        }
        if (save_step > 0 && (samp + 1) % save_step == 0) {    // periodically save
            capture();
            post_epoch_callback(samp + 1);
        }
    }
    capture();
}

void ShadowMap::estimateShadow(const Intersection &surface, real &Wlv, real &Wl) {
    if (!surface.hit->mtr->emi.isBlack()) { // first hit is light
        Wlv = Wl = 1;
        return;
    }
    Dir ex, ey, ez = surface.n;
    ez.getOrthogonalBasis(ex, ey);
    auto samp = Sampling::uniformOnHemisphere({randf(), randf()});
    Dir direction = ex * samp.x + ey * samp.y + ez * samp.z;
    // sample a direction on the hemisphere determined by the surface normal
    Ray ray(surface.pos + direction * EPS, direction);
    Intersection isect;
    // 1st test
    if (scene.intersectAny(ray, isect, true)) {
        if (!isect.hit->mtr->emi.isBlack()) { // if hit at some light
            Wlv = Wl = 1;
        }
        else {  // hit at some non-light occlusion
            Wlv = 0;
            // 2nd test
            Wl = scene.intersectAnyLight(ray) ? 1 : 0; // *possibly* hit some potential light?
        }
    }
    else { // a missed sample
        Wlv = Wl = 0;
    }
}

void ShadowMap::capture() {
    // compute shadow values
#if OMP_ON
#pragma omp parallel for schedule(dynamic, 1)
#endif
    for (size_t j = 0; j < camera.height; ++j) {
        for (size_t i = 0; i < camera.width; ++i) {
            if (Wl_buffer[i][j] == 0) {
                camera.draw(i, j, Color(0.5, 0, 0)); // NaN
            }
            else {
                real shadow = clamp(Wlv_buffer[i][j] / Wl_buffer[i][j]);
                camera.draw(i, j, Color(shadow, shadow, shadow));
            }
        }
    }
}
