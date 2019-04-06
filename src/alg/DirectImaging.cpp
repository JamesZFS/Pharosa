//
// Created by James on 2019/4/6.
//

#include "DirectImaging.h"

DirectImaging::DirectImaging(Stage &stage_, Cameras::Camera &camera_) : Illuminator(stage_, camera_)
{}

Color DirectImaging::radiance(const Ray &ray, unsigned int depth) const
{
	Pos x;
	Dir normal;
	const Scenes::Object *hit = nullptr;
	if (!stage.intersectAny(ray, hit, x, normal)) return Color::BLACK;
	Dir nl = normal % ray.dir < 0 ? normal : normal * -1;    // regularized normal, against in direction
	// return the RGB color of hit, assuming environment light source is shooting at (-1, -1, -1)
	return hit->color * (0.1 + 0.45 * (1 + Dir::UNIT % nl)) + hit->emi;
}
