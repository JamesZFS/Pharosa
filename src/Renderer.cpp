//
// Created by James on 2019/4/9.
//

#include "Renderer.h"
#include "scene/Scene.h"
#include "camera/All.h"
#include "alg/All.h"
#include "utils/funcs.hpp"
#include <omp.h>

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

	printf("\n----------------------------------------------------------------\n");
	printf("loaded %ld singletons, %ld bounding boxes in total.\n",
		   scene->getSingletonCount(), scene->getBoundingBoxCount());
	printf("camera viewpoint at %s  orienting towards %s\n",
		   camera->viewpoint().toString().data(), camera->orientation().toString().data());
	printf("algorithm info: %s", algorithm->info().data());
	printf("\n---------------------- ready to render -------------------------\n\n");
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
