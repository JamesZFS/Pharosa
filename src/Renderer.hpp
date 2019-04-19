//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_RENDERER_HPP
#define PHAROSA_RENDERER_HPP

#include "lib.h"
#include "core/Vec.h"
#include "core/Ray.hpp"
#include "alg/All.h"
#include "camera/All.h"
#include "scene/Stage.h"

// frontend of all, encapsulates a GI Algorithm, a Camera and a Scene Stage
// todo move template declaration to setup functions
template<typename Render_Algorithm, typename Camera_Type>
class Renderer
{
private:
	Stage *_stage;
	Camera *_camera;
	Algorithm *_illuminator;
	size_t prev_epoch;    // previous rendering times

	void setupAlgorithm();

	// render one time frame
	void renderFrame(size_t n_epoch, size_t verbose_step, const String &checkpoint_dir);

public:
	// init a Renderer with any type of Alg and Camera
	Renderer();

	~Renderer();

	// access to stage and camera
	Stage &stage();

	Camera &camera();

	// setup stage by specify a stage config file path
	void setupStage(const String &config_path = "");

	// setup stage by designating a stage config file path, can continue from a previous ppm image path
	void setupCamera(const Pos &pos_, const ElAg &euler_angles_, size_t width_ = 1024, size_t height_ = 768,
					 const String &prev_path_ = "", size_t prev_epoch_ = 0);

	/** start rendering
	 *
	 * @param n_epoch : epochs to render
	 * @param verbose_step : steps to update progressbar, if 0 - no progressbar
	 * @param checkpoint_dir : directory for storing checkpoint ppm images, if "" - don't save checkpoints
	 */
	void start(size_t n_epoch = 1, size_t verbose_step = 10000, const String &checkpoint_dir = "");

	/** start a kinetic rendering
	 *
	 * @param n_frame : time frames
	 * @param motion : will execute after rendering each time frame
	 * @param n_epoch : number of samples to render each time frame
	 * @param verbose_step
	 * @param checkpoint_dir : directory for storing checkpoint ppm images for each frame, if "" - don't save checkpoints
	 */
	void startKinetic(size_t n_frame, void (*motion)(), size_t n_epoch = 1,
					  size_t verbose_step = 10000, const String &checkpoint_dir = "");

	// save ppm image
	void save(const String &out_path);
};

#include "Renderer.tcc"

#endif //PHAROSA_RENDERER_HPP
