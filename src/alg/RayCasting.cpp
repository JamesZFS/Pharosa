//
// Created by James on 2019/4/6.
//

#include "RayCasting.h"
#include "../scene/Scene.h"
#include "../utils/funcs.hpp"

RayCasting::RayCasting(Scene &scene_, const Dir &light_dir_) :
	Algorithm(scene_), light_dir(light_dir_)
{
}

Color RayCasting::radiance(const Ray &ray) const
{
	Intersection isect;
	if (!scene.intersectAny(ray, isect)) return Color::BLACK;
	Dir nl = isect.normal % ray.dir >= 0 ? isect.normal : -isect.normal;    // regularized normal, parallel to incidence
	// return the RGB color of hit, assuming environment light is shooting towards (-1, -1, -1)
	return isect.getColor() * (0.1 + 0.9 * Funcs::clamp(light_dir % nl)) + isect.getEmission();
}

String RayCasting::info() const
{
	return "<Ray Casting>";
}
