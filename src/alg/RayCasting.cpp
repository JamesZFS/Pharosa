//
// Created by James on 2019/4/6.
//

#include "RayCasting.h"
#include "../utils/funcs.hpp"

Dir RayCasting::LIGHT_DIR(-1, -1, -1);

RayCasting::RayCasting(Stage &stage_, Camera &camera_) : Algorithm(stage_, camera_)
{
}

Color RayCasting::radiance(const Ray &ray, size_t depth) const
{
	Pos x;
	Dir normal;
	const Object *hit = nullptr;
	if (!stage.intersectAny(ray, hit, x, normal)) return Color::BLACK;
	Dir nl = normal % ray.dir >= 0 ? normal : normal * -1;    // regularized normal, parallel to incidence
	// return the RGB color of hit, assuming environment light is shooting towards (-1, -1, -1)
	return hit->color * (0.1 + 0.9 * Funcs::clamp(LIGHT_DIR % nl)) + hit->emi;
}
