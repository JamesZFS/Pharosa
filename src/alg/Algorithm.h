//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_ALGORITHM_H
#define PHAROSA_ALGORITHM_H

#include "../Pharosa.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"

#include <functional>

class Scene;

class Camera;

// standard illumination algorithm, base class, can access scene and camera
class Algorithm
{
protected:
	const Scene &scene;
	Camera &camera;

public:
	Algorithm(const Scene &scene_, Camera &camera_);

	virtual ~Algorithm() = default;

	virtual void start(size_t n_epoch,
					   const std::function<void(size_t)> &pre_epoch_callback,
					   const std::function<void(size_t)> &in_epoch_callback,
					   const std::function<void(size_t)> &post_epoch_callback);    // entrance

	virtual String info() const = 0;

	virtual Color radiance(const Ray &ray) const = 0;    // main

	static Algorithm *acquire(const Json &json, const Scene &scene, Camera &camera);
};

#endif //PHAROSA_ALGORITHM_H
