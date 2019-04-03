//
// Created by think on 2019/4/1.
//

#include "Renderer.h"

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::Renderer() :
		stage(nullptr), camera(nullptr), illuminator(nullptr)
{
}

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::~Renderer()
{
	delete stage;
	delete camera;
	delete illuminator;
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::setupStage(const String &config_path)
{
	if (stage != nullptr) delete stage;
	stage = new Stage;
	stage->fromJsonFile(config_path);
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::setupStage(ObjectGroup &&objects)
{
	if (stage != nullptr) delete stage;
	stage = new Stage;
	stage->fromList(objects);
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::
setupCamera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_,
			const String &prev_path_, unsigned int prev_epoch_)
{
	prev_epoch = prev_epoch_;
	if (camera != nullptr) delete camera;
	camera = new Cameras_Type(pos_, euler_angles_, width_, height_);

	// load from prev
	if (prev_path_.length() > 0) {
		camera->readPPM(prev_path_, prev_epoch_);
	}
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::start(unsigned int n_epoch,
												 unsigned int verbose_step, const String &checkpoint_dir)
{
	if (stage == nullptr) {
		warn("Warning: stage is not setup yet, the result shall be empty.\n");
		stage = new Stage;
	}
	if (camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	// setup algorithm
	if (illuminator == nullptr) {
		illuminator = new GI_Algorithm(*stage, *camera);
	}

	illuminator->render(n_epoch, prev_epoch, verbose_step, checkpoint_dir);
	prev_epoch += n_epoch;
}

template<typename GI_Algorithm, typename Cameras_Type>
void Renderer<GI_Algorithm, Cameras_Type>::save(const String &out_path)
{
	if (camera == nullptr) {
		warn("Error: camera is not setup yet.\n");
		exit(1);
	}
	camera->writePPM(out_path);
}
