//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_RENDERER_H
#define PHAROSA_RENDERER_H

#include "lib.h"
#include "Vec.hpp"
#include "Ray.hpp"
#include "scene/Stage.h"
#include "camera/All.h"
#include "alg/All.h"

// frontend of all, encapsulates a GI Algorithm, a Camera and a Scene Stage
template<typename Render_Algorithm, typename Cameras_Type>
class Renderer
{
private:
	Stage *_stage;
	Cameras::Camera *_camera;
	Algorithms::Illuminator *_illuminator;
	unsigned int prev_epoch;    // previous rendering times

	void setupAlgorithm();

	// render one time frame
	void renderFrame(unsigned int n_epoch, unsigned int verbose_step, const String &checkpoint_dir);

public:
	// init a Renderer with any type of Alg and Camera
	Renderer();

	~Renderer();

	// access to stage and camera
	inline Stage &stage();

	inline const Cameras::Camera &camera();

	// setup stage by specify a stage config file path
	void setupStage(const String &config_path = "");

	// setup stage by designating a stage config file path, can continue from a previous ppm image path
	void setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768,
					 const String &prev_path_ = "", unsigned int prev_epoch_ = 0);

	/** start rendering
	 *
	 * @param n_epoch : epochs to render
	 * @param verbose_step : steps to update progressbar, if 0 - no progressbar
	 * @param checkpoint_dir : directory for storing checkpoint ppm images, if "" - don't save checkpoints
	 */
	void start(unsigned int n_epoch = 1, unsigned int verbose_step = 10000, const String &checkpoint_dir = "");

	/** start a kinetic rendering
	 *
	 * @param n_frame : time frames
	 * @param motion : will execute after rendering each time frame
	 * @param n_epoch : number of samples to render each time frame
	 * @param verbose_step
	 * @param checkpoint_dir : directory for storing checkpoint ppm images for each frame, if "" - don't save checkpoints
	 */
	void startKinetic(unsigned int n_frame, void (*motion)(), unsigned int n_epoch = 1,
					  unsigned int verbose_step = 10000, const String &checkpoint_dir = "");

	// save ppm image
	void save(const String &out_path);
};

#include "Renderer.cpp"

#endif //PHAROSA_RENDERER_H
