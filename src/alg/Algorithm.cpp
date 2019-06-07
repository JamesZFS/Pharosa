//
// Created by James on 2019/4/2.
//

#include "Algorithm.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"

#include <omp.h>

#define OMP_ON 1


Algorithm::Algorithm(const Scene &scene_, Camera &camera_) : scene(scene_), camera(camera_)
{
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
