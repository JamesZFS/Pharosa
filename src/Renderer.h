//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_RENDERER_H
#define PHAROSA_RENDERER_H

#include "Pharosa.h"

#define __USE_OMP__

class Scene;

class Camera;

class Algorithm;

// frontend of all, encapsulates a GI Algorithm, a Camera and a Scene Scene
class Renderer
{
private:
	Scene *scene;
	Camera *camera;
	Algorithm *algorithm;

	// path to save ppm file, will also write checkpoint to `save_path.cpt`
	String save_path;
	// path to load previously rendered ppm file, will also try to load checkpoint from `prev_path.cpt`
	String prev_path;

	size_t n_epoch;            // epochs to render
	size_t save_step;        // save every `save_step` epoch, if 0 - no saving through out rendering
	size_t verbose_step;    // steps to update progressbar, if 0 - no progressbar

	void getReady();

	void checkIfReady();

	// render one time frame
	void renderFrame();

	void saveProgress(size_t cur_epoch) const;    // save progress without checking

	// ** rendering pipeline **
	// do the rendering for n_epoch times, callback each epoch
	void render();

	void renderVerbose();

public:
	Renderer();

	Renderer(const String &config_path);    // init from json

	Renderer(const Json &json);

	~Renderer();

	void setup(const Json &json);    // re-setup from json

	void clear();

	// start rendering
	void start();

	/** start a kinetic rendering
	 *
	 * @param n_frame : time frames
	 * @param motion : will execute after rendering each time frame
	 */
	void startKinetic(size_t n_frame, void (*motion)());

	// save ppm image
	void save() const;
};

#endif //PHAROSA_RENDERER_H
