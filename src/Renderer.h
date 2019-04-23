//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_RENDERER_H
#define PHAROSA_RENDERER_H

#include "lib.h"
#include "core/Vec.h"
#include "core/Ray.hpp"
#include "alg/All.h"
#include "camera/All.h"
#include "scene/Scene.h"

// frontend of all, encapsulates a GI Algorithm, a Camera and a Scene Scene
// todo move template declaration to setup functions
class Renderer
{
#ifdef __DEV_STAGE__
public:
#else
private:
#endif
	Scene *scene;
	Camera *camera;
	Algorithm *algorithm;

	String save_path;
	String checkpoint_dir;
	String prev_path;
	/**
	 * n_epoch : epochs to render
	 * prev_epoch : previous rendering times
	 * verbose_step : steps to update progressbar, if 0 - no progressbar
	 * checkpoint_dir : directory for storing checkpoint ppm images, if "" - don't save checkpoints
	 */
	size_t n_epoch;
	size_t prev_epoch;
	size_t verbose_step;

	void checkIfReady();

	// render one time frame
	void renderFrame();

public:
	Renderer(const String &config_path);    // init from json

	~Renderer();

	// start rendering
	void start();

	/** start a kinetic rendering
	 *
	 * @param n_frame : time frames
	 * @param motion : will execute after rendering each time frame
	 */
	void startKinetic(size_t n_frame, void (*motion)());

	// save ppm image
	void save();
};

#endif //PHAROSA_RENDERER_H
