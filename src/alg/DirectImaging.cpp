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

void DirectImaging::render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir)
{
	Illuminator::render(1, prev_epoch, checkpoint_dir);	// one epoch is enough
}

void DirectImaging::renderVerbose(unsigned int n_epoch, unsigned int prev_epoch, unsigned int verbose_step,
								  const String &checkpoint_dir)
{
	Illuminator::renderVerbose(1, prev_epoch, verbose_step, checkpoint_dir);
}
