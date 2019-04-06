//
// Created by think on 2019/4/1.
//

#include "Renderer.h"

template<typename Render_Algorithm, typename Cameras_Type>
Renderer<Render_Algorithm, Cameras_Type>::Renderer() :
		_stage(nullptr), _camera(nullptr), _illuminator(nullptr)
{
}

template<typename Render_Algorithm, typename Cameras_Type>
Renderer<Render_Algorithm, Cameras_Type>::~Renderer()
{
	delete _stage;
	delete _camera;
	delete _illuminator;
}

template<typename Render_Algorithm, typename Cameras_Type>
Stage &Renderer<Render_Algorithm, Cameras_Type>::stage()
{
	if (_stage == nullptr) {
		warn("Error: stage is not setup yet.\n");
		exit(1);
	}
	return *_stage;
}

template<typename Render_Algorithm, typename Cameras_Type>
const Cameras::Camera &Renderer<Render_Algorithm, Cameras_Type>::camera()
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	return *_camera;
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::
setupStage(const String &config_path)
{
	if (_stage != nullptr) delete _stage;
	_stage = new Stage;            // new an empty stage
	if (config_path.length() > 0) {
		_stage->fromJsonFile(config_path);
	}
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::
setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_,
			const String &prev_path_, unsigned int prev_epoch_)
{
	prev_epoch = prev_epoch_;
	if (_camera != nullptr) delete _camera;
	_camera = new Cameras_Type(pos_, euler_angles_, width_, height_);    // new a camera

	// load from prev
	if (prev_path_.length() > 0) {
		_camera->readPPM(prev_path_, prev_epoch_);
	}
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::
setupAlgorithm()
{
	if (_stage == nullptr) {
		warn("Warning: stage is not setup yet, the result shall be empty.\n");
		_stage = new Stage;
	}
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	// setup algorithm
	if (_illuminator == nullptr) {
		_illuminator = new Render_Algorithm(*_stage, *_camera);
	}
	debug("loaded %d objects in total.\n", _stage->getObjectCount());
	debug("camera viewpoint at %s  orienting towards %s\n",
		  camera().viewpoint().toString().data(), camera().orientation().toString().data());
	debug("ready to render.\n");
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::renderFrame(unsigned int n_epoch, unsigned int verbose_step,
														   const String &checkpoint_dir)
{
	if (verbose_step == 0)
		_illuminator->render(n_epoch, prev_epoch, checkpoint_dir);
	else
		_illuminator->renderVerbose(n_epoch, prev_epoch, verbose_step, checkpoint_dir);
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::start(unsigned int n_epoch,
													 unsigned int verbose_step, const String &checkpoint_dir)
{
	setupAlgorithm();
	debug("===== rendering start =====\n");
	double since = omp_get_wtime();

	renderFrame(n_epoch, verbose_step, checkpoint_dir);

	auto elapse = lround(omp_get_wtime() - since);
	debug("\n===== rendering finished in %ld min %ld sec =====\n", elapse / 60, elapse % 60);
	prev_epoch += n_epoch;
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::
startKinetic(unsigned int n_frame, void (*motion)(), unsigned int n_epoch,
			 unsigned int verbose_step, const String &checkpoint_dir)
{
	if (prev_epoch > 0) {
		warn("Error: before rendering kinetic images, you should not pre-read from previous image.\n");
		exit(1);
	}
	setupAlgorithm();
	bool checkpoint = (checkpoint_dir.length() > 0);
	debug("======= kinetic rendering start =======\n");
	double since = omp_get_wtime();

	for (unsigned int frame = 0; frame < n_frame; ++frame) {
		debug("\n===== frame %d / %d =====\n", frame + 1, n_frame);
		renderFrame(n_epoch, verbose_step, "");
		motion();    // deal with motion
		if (checkpoint) {    // save checkpoint for each frame
			Buffer out_path;
			sprintf(out_path, "%s/frame - %d (samps = %d).ppm", checkpoint_dir.data(), frame + 1, n_epoch);
			_camera->writePPM(out_path);
		}
	}

	auto elapse = lround(omp_get_wtime() - since);
	debug("\n======= kinetic rendering finished in %ld min %ld sec =======\n", elapse / 60, elapse % 60);
}

template<typename Render_Algorithm, typename Cameras_Type>
void Renderer<Render_Algorithm, Cameras_Type>::save(const String &out_path)
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	_camera->writePPM(out_path);
}
