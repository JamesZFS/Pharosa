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
public:
	// init a Renderer with any type of Alg and Camera
	Renderer();

	~Renderer();

	// setup stage by specify a stage config file path
	void setupStage(const String &config_path);

	// setup stage by designating a stage config file path
	void setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	// start rendering
	void start(unsigned int n_epoch = 1, unsigned int verbose = 1, bool save_checkpoints = false);

	// save ppm image
	void save(const String &out_path);
};

#include "Renderer.cpp"

#endif //RAYTRACKER_RENDERER_H
