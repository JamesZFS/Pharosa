//
// Created by James on 2019/4/6.
//

#ifndef PHAROSA_DIRECTIMAGING_H
#define PHAROSA_DIRECTIMAGING_H

#include "Illuminator.h"

// Direct Imaging - render directly the RGB color of an object
class DirectImaging : public Illuminator
{
protected:
	Color radiance(const Ray &ray, unsigned int depth) const override;

public:
	DirectImaging(Stage &stage_, Cameras::Camera &camera_);

	void render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir) override;

	void renderVerbose(unsigned int n_epoch, unsigned int prev_epoch, unsigned int verbose_step,
					   const String &checkpoint_dir) override;
};

#include "DirectImaging.cpp"

#endif //PHAROSA_DIRECTIMAGING_H
