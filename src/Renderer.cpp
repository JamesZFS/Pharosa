//
// Created by think on 2019/4/1.
//

#include "Renderer.h"

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::Renderer() :
		_stage(nullptr), _camera(nullptr), _illuminator(nullptr)
{
}

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::~Renderer()
{
	delete _stage;
	delete _camera;
	delete _illuminator;
}

template<typename GI_Algorithm, typename Cameras_Type>
Stage &Renderer<GI_Algorithm, Cameras_Type>::stage()
{
	if (_stage == nullptr) {
		warn("Error: stage is not setup yet.\n");
		exit(1);
	}
	return *_stage;
}

template<typename GI_Algorithm, typename Cameras_Type>
const Cameras::Camera &Renderer<GI_Algorithm, Cameras_Type>::camera()
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	return *_camera;
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::
setupStage(const String &config_path)
{
	if (_stage != nullptr) delete _stage;
	_stage = new Stage;            // new an empty stage
	if (config_path.length() > 0) {
		_stage->fromJsonFile(config_path);
	}
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::
setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_,
			const String &prev_path_, unsigned int prev_epoch_)
{
	prev_epoch = prev_epoch_;
	if (_camera != nullptr) delete _camera;
	_camera = new Cameras_Type(pos_, euler_angles_, width_, height_);	// new a camera

	// load from prev
	if (prev_path_.length() > 0) {
		_camera->readPPM(prev_path_, prev_epoch_);
	}
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::start(unsigned int n_epoch,
												 unsigned int verbose_step, const String &checkpoint_dir)
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
		_illuminator = new GI_Algorithm(*_stage, *_camera);
	}

	debug("loaded %d objects in total.\n", _stage->getObjectCount());
	debug("camera viewpoint at %s  orienting towards %s\n",
			camera().viewpoint().toString().data(), camera().orientation().toString().data());
	debug("===== rendering start =====\n");
	double since = omp_get_wtime();

	_illuminator->render(n_epoch, prev_epoch, verbose_step, checkpoint_dir);

	auto elapse = lround(omp_get_wtime() - since);
	debug("\n===== rendering finished in %ld min %ld sec =====\n", elapse / 60,  elapse % 60);
	prev_epoch += n_epoch;
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::save(const String &out_path)
{
	if (_camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	_camera->writePPM(out_path);
}
