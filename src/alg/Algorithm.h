//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_ALGORITHM_H
#define PHAROSA_ALGORITHM_H

#include "../Pharosa.h"
#include "../core/Vec.h"
#include "../core/Ray.hpp"

class Scene;

// standard illumination algorithm, base class, can access scene and camera
class Algorithm
{
protected:
	const Scene &scene;

public:
	Algorithm(const Scene &scene_);

	virtual ~Algorithm() = default;

	virtual String info() const = 0;

	virtual Color radiance(const Ray &ray) const = 0;    // entrance

	static Algorithm *acquire(const Json &json, Scene &scene);
};

#endif //PHAROSA_ALGORITHM_H
