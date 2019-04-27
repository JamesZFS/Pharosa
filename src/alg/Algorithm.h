//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_ALGORITHM_H
#define PHAROSA_ALGORITHM_H

#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"

class Scene;

// standard illumination algorithm, base class, can access scene and camera
class Algorithm
{
protected:
	const Scene &scene;

	// interfaces:
	virtual Color radiance(const Ray &ray, size_t depth) const = 0;    // ** main algorithm **

public:
	Algorithm(const Scene &scene_);

	virtual String info() const = 0;

	inline Color radiance(const Ray &ray) const    // entrance
	{ return radiance(ray, 0); }

	static Algorithm *acquire(const Json &json, Scene &scene);
};

#endif //PHAROSA_ALGORITHM_H
