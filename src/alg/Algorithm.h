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
class Intersection;

// standard illumination algorithm, base class, can access scene and camera
class Algorithm
{
protected:
	const Scene &scene;
	Camera &camera;

	/** specially sample all **sphere** light sources:
	 * for all sphere light sources
	 * sphere-cap-ly sample a ray from current intersection
	 * make sure the ray doesn't hit another object, even if that is a light source too.
	 * take the hit light source's emission into I_in
	 */
	Color getDirectLighting(const Intersection &isect) const;

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
