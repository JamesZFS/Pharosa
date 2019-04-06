//
// Created by James on 2019/4/6.
//

#include "../utils/funcs.hpp"
#include "RayCasting.h"

Dir RayCasting::LIGHT_DIR(-1, -1, -1); 

RayCasting::RayCasting(Stage &stage_, Cameras::Camera &camera_) : Illuminator(stage_, camera_)
{
}

Color RayCasting::radiance(const Ray &ray, unsigned int depth) const
{
	Pos x;
	Dir normal;
	const Scenes::Object *hit = nullptr;
	if (!stage.intersectAny(ray, hit, x, normal)) return Color::BLACK;
	Dir nl = normal % ray.dir >= 0 ? normal : normal * -1;    // regularized normal, parallel to incidence
	// return the RGB color of hit, assuming environment light is shooting towards (-1, -1, -1)
	return hit->color * (0.1 + 0.9 * Funcs::clamp(LIGHT_DIR % nl)) + hit->emi;
}
