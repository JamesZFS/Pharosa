//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_RENDERER_H
#define RAYTRACKER_RENDERER_H

#include "lib.h"
#include "Vec.hpp"
#include "Ray.hpp"
#include "scene/Stage.h"
#include "camera/All.h"
#include "alg/All.h"

// frontend of all, encapsulates a GI Algorithm, a Camera and a Scene Stage
template<typename GI_Algorithm, typename Cameras_Type>
class Renderer
{
private:
	Stage *stage;
	Cameras::Camera *camera;
	Algorithms::GI *illuminator;
	unsigned int prev_epoch;    // previous rendering times

public:
	// init a Renderer with any type of Alg and Camera
	Renderer();

	~Renderer();

	// setup stage by specify a stage config file path
	void setupStage(const String &config_path);

	void setupStage(ObjectGroup &&objects);

	// setup stage by designating a stage config file path, can continue from a previous ppm image path
	void setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768,
					 const String &prev_path_ = "", unsigned int prev_epoch_ = 0);

	/** start rendering
	 *
	 * @param n_epoch : epochs to render
	 * @param verbose_step : steps to update progressbar, if 0 - no progressbar
	 * @param checkpoint_dir : directory for storing checkpoint ppm images, should not end with "/", if "" - don't save checkpoints
	 */
	void start(unsigned int n_epoch = 1, unsigned int verbose_step = 10000, const String &checkpoint_dir = "");

	// save ppm image
	void save(const String &out_path);
};

#include "Renderer.cpp"

#endif //RAYTRACKER_RENDERER_H
