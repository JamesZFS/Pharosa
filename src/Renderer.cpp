//
// Created by James on 2019/4/9.
//

#include "Renderer.h"
#include "utils/parsers/json.hpp"
#include <omp.h>

Renderer::Renderer(const String &config_path)
{
	Json json;
	std::ifstream fin;
	fin.open(config_path, std::ios::in);

	if (!fin.is_open()) {    // exception
		fin.close();
		TERMINATE("IO Error: config_path \"%s\" cannot be opened, reading stopped.", config_path.data());
	}

	fin >> json;
	fin.close();

	// parsing json
	String type;

	save_path = json.at("save_path");
	checkpoint_dir = json.value("checkpoint_dir", "");    // get with default value
	prev_path = json.value("prev_path", "");
	n_epoch = json.at("n_epoch");
	prev_epoch = json.value("prev_epoch", 0);
	verbose_step = json.value("verbose_step", 0);
	Funcs::generator.seed((unsigned int) time(nullptr));

	// camera
	Json cam_json = json.at("camera");
	type = cam_json.value("type", "basic");
	if (type == "basic") {
		camera = new BasicCamera(cam_json);
	}
	else if (type == "ortho") {
		camera = new OrthoCamera(cam_json);
	}
	else if (type == "DOF") {
		//	camera = new DOFCamera(cam_json);
	}
	else TERMINATE("Error: got unidentified camera type \"%s\".", type.data());
	// load checkpoint
	if (prev_path.length() > 0) {
		camera->readPPM(prev_path, prev_epoch);
	}

//	message(cam_json.dump(3));

	// scene
	Json scene_json = json.at("scene");
	scene = new Scene(scene_json);

//	message(scene_json.dump(3));

	// algorithm
	Json alg_json = json.at("algorithm");
	type = alg_json.at("type");
	if (type == "RayCasting" || type == "ray casting" || type == "RC") {
		try {
			algorithm = new RayCasting(*scene, *camera, Dir(alg_json.at("light_dir")));
		}
		catch (Json::out_of_range &) {
			algorithm = new RayCasting(*scene, *camera);	// use default
		}
	}
	else if (type == "RayTracing" || type == "ray tracing" || type == "RT") {
		try {
			algorithm = new RayTracing(*scene, *camera, (size_t) alg_json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			algorithm = new RayTracing(*scene, *camera);	// use default
		}
	}
	else TERMINATE("Error: got unidentified algorithm type \"%s\".", type.data());

	algorithm->report();
//	message(alg_json.dump(3));
}


Renderer::~Renderer()
{
	delete scene;
	delete camera;
	delete algorithm;
}

void Renderer::checkIfReady()
{
	if (scene == nullptr) TERMINATE("Error: scene is not setup yet.\n");
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.\n");
	if (algorithm == nullptr) TERMINATE("Error: algorithm is not setup yet.\n");

	printf("loaded %ld singletons, %ld bounding boxes in total.\n",
		   scene->getSingletonCount(), scene->getBoundingBoxCount());
	printf("camera viewpoint at %s  orienting towards %s\n",
		   camera->viewpoint().toString().data(), camera->orientation().toString().data());
	printf("ready to render.\n");
}


void Renderer::renderFrame()
{
	if (verbose_step == 0)
		algorithm->render(n_epoch, prev_epoch, checkpoint_dir);
	else
		algorithm->renderVerbose(n_epoch, prev_epoch, verbose_step, checkpoint_dir);
}

void Renderer::start()
{
	checkIfReady();

	printf("===== rendering start =====\n");
	if (checkpoint_dir.length() > 0) {
		printf("writing to checkpoint \"%s\"\n", checkpoint_dir.data());
	}

	double since = omp_get_wtime();
	renderFrame();
	auto elapse = lround(omp_get_wtime() - since);

	printf("\n===== rendering finished in %ld min %ld sec =====\n", elapse / 60, elapse % 60);
	prev_epoch += n_epoch;
}

void Renderer::startKinetic(size_t n_frame, void (*motion)())
{
	if (prev_epoch > 0) {
		warn("Error: before rendering kinetic images, you should not pre-read from previous image.\n");
		exit(1);
	}
	checkIfReady();

	bool checkpoint = (checkpoint_dir.length() > 0);
	if (checkpoint) {
		printf("writing to checkpoint \"%s\"\n", checkpoint_dir.data());
	}
	printf("======= kinetic rendering start =======\n");

	double since = omp_get_wtime();
	for (size_t frame = 0; frame < n_frame; ++frame) {
		printf("\n===== frame %ld / %ld =====\n", frame + 1, n_frame);
		renderFrame();
		motion();    // deal with motion
		if (checkpoint) {    // save checkpoint for each frame
			Buffer out_path;
			sprintf(out_path, "%s/frame - %ld (samps = %ld).ppm", checkpoint_dir.data(), frame + 1, n_epoch);
			camera->writePPM(out_path);
		}
	}
	auto elapse = lround(omp_get_wtime() - since);

	printf("\n======= kinetic rendering finished in %ld min %ld sec =======\n", elapse / 60, elapse % 60);
}

void Renderer::save()
{
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.\n");
	camera->writePPM(save_path);
	printf("image written to \"%s\" \n", save_path.data());
}
