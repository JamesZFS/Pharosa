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
	// return the RGB color of hit
	return hit->color + hit->emi;
}
