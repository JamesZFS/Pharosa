//
// Created by James on 2019/4/6.
//

#include "RayCasting.h"
#include "../scene/Scene.h"
#include "../utils/funcs.hpp"

RayCasting::RayCasting(Scene &scene_, Camera &camera_, const Dir &light_dir_) :
	Algorithm(scene_, camera_), light_dir(light_dir_)
{
}

Color RayCasting::radiance(const Ray &ray, size_t depth) const
{
	Pos x;
	Dir normal;
	const Object *hit = nullptr;
	if (!scene.intersectAny(ray, hit, x, normal)) return Color::BLACK;
	Dir nl = normal % ray.dir >= 0 ? normal : normal * -1;    // regularized normal, parallel to incidence
	// return the RGB color of hit, assuming environment light is shooting towards (-1, -1, -1)
	return hit->color * (0.1 + 0.9 * Funcs::clamp(light_dir % nl)) + hit->emi;
}

String RayCasting::info()
{
	return "<Ray Casting>";
}
