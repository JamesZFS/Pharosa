//
// Created by James on 2019/4/9.
//

#include "Renderer.h"
#include "scene/Scene.h"
#include "camera/All.h"
#include "alg/All.h"
#include "utils/funcs.hpp"
#include <fstream>
#include <omp.h>

Renderer::~Renderer()
{
	delete scene;
	delete camera;
}

void Renderer::getReady()
{
	if (prev_ppm_path.length() > 0) {    // load from checkpoint
		camera->readPPM(prev_ppm_path, prev_cpt_path);
		printf("loaded previous status from \"%s\"\n", prev_ppm_path.data());
	}
}

void Renderer::checkIfReady()
{
	if (scene == nullptr) TERMINATE("Error: scene is not setup yet.")
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.")
	if (algorithm == nullptr) TERMINATE("Error: algorithm is not setup yet.")

	printf("\n----------------------------------------------------------------\n");
	printf("loaded %ld objects, %ld triangle meshes in total.\n",
		   scene->getSingletonCount(), scene->getMeshCount());
	printf("camera viewpoint at %s  orienting towards %s\n",
		   camera->viewpoint().toString().data(), camera->orientation().toString().data());
	printf("algorithm info: %s", algorithm->info().data());
	printf("\n---------------------- ready to render -------------------------\n\n");
}

void Renderer::renderFrame()
{
	if (verbose_step == 0)
		render();
	else
		renderVerbose();
}

void Renderer::start()
{
	checkIfReady();
	scene->buildKDTree();
	printf("===== rendering start =====\n");

	double since = omp_get_wtime();
	renderFrame();
	auto elapse = lround(omp_get_wtime() - since);

	printf("\n===== rendering finished in %ld min %ld sec =====\n", elapse / 60, elapse % 60);
}

void Renderer::startKinetic(size_t n_frame, void (*motion)())
{
//	if (prev_cpt_path.length() > 0) {
//		TERMINATE("Error: before rendering kinetic images, you should not pre-read from previous image.");
//	}
//	checkIfReady();
//
//	printf("======= kinetic rendering start =======\n");
//
//	double since = omp_get_wtime();
//	for (size_t frame = 0; frame < n_frame; ++frame) {
//		printf("\n===== frame %ld / %ld =====\n", frame + 1, n_frame);
//		renderFrame();
//		motion();    // deal with motion
//		if (checkpoint) {    // save checkpoint for each frame
//			Buffer out_path;
//			sprintf(out_path, "%s/frame - %ld (samps = %ld).ppm", checkpoint_dir.data(), frame + 1, n_epoch);
//			camera->writePPM(out_path);
//		}
//	}
//	auto elapse = lround(omp_get_wtime() - since);
//
//	printf("\n======= kinetic rendering finished in %ld min %ld sec =======\n", elapse / 60, elapse % 60);
}

void Renderer::save() const
{
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.");
	camera->writePPM(save_ppm_path, save_cpt_path);
	printf("image written to \"%s\" \n", save_ppm_path.data());
}

void Renderer::saveProgress(size_t cur_epoch) const
{
	if (save_step == 0 || cur_epoch % save_step > 0) return;
	camera->writePPM(save_ppm_path, save_cpt_path);
	barInfo("\t progress saved to \"%s\"\n", save_ppm_path.data());
}

void Renderer::render()
{
// without progressbar, fast version

//	detectEdges();
	double since = omp_get_wtime();

	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {    // for samples
		auto eta = lround((omp_get_wtime() - since) * (n_epoch - epoch) / epoch);
		barInfo("\r=== epoch %ld / %ld ===  eta: %ld min %ld sec", epoch + 1, n_epoch, eta / 60, eta % 60);

#ifdef __USE_OMP__
#pragma omp parallel for schedule(dynamic, 1)
#endif
		for (size_t j = 0; j < camera->height; ++j) {                // for each pixel
			for (size_t i = 0, rank = j * camera->width; i < camera->width; ++i, ++rank) {
				camera->render(rank, algorithm->radiance(camera->shootRayAt(i, j, 0.5)));    // rand normal AA
			}
		}
		saveProgress(epoch + 1);
	}
	printf("\n");
}

void Renderer::renderVerbose()
{
	// with progressbar

	double since = omp_get_wtime();

	for (size_t epoch = 0; epoch < n_epoch; ++epoch) {    // for samples
		auto eta = lround((omp_get_wtime() - since) * (n_epoch - epoch) / epoch);
		barInfo("=== epoch %ld / %ld ===  eta: %ld min %ld sec\n", epoch + 1, n_epoch, eta / 60, eta % 60);

#ifdef __USE_OMP__
#pragma omp parallel for schedule(dynamic, 1)
#endif

		for (size_t j = 0; j < camera->height; ++j) {                // for each pixel
			if (j % verbose_step == 0) {
				barInfo("\r %.1f %%", j * 100.0 / camera->height);    // progressbar :)
			}
			for (size_t i = 0, rank = j * camera->width; i < camera->width; ++i, ++rank) {
				camera->render(rank, algorithm->radiance(camera->shootRayAt(i, j, 0.5)));    // rand normal AA
			}
		}
		saveProgress(epoch + 1);
		barInfo("\n");
	}
}
