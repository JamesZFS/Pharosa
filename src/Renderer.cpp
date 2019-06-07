//
// Created by James on 2019/4/9.
//

#include "Renderer.h"
#include "scene/Scene.h"
#include "camera/All.h"
#include "alg/All.h"
#include "utils/funcs.hpp"
#include <omp.h>

Renderer::Renderer() : scene(nullptr), camera(nullptr), algorithm(nullptr)
{

}

Renderer::Renderer(const Json &json) : Renderer()
{
	setup(json);
}

Renderer::~Renderer()
{
	clear();
}

void Renderer::clear()
{
	delete scene;
	delete camera;
	delete algorithm;
	scene = nullptr;
	camera = nullptr;
	algorithm = nullptr;
	save_path = prev_path = "";
	n_epoch = save_step = verbose_step = 0;
}

void Renderer::getReady()
{
	if (prev_path.length() > 0) {    // load from checkpoint
		camera->readPPM(prev_path);
		printf("\033[32mloaded previous status from \"%s\", previous draw count = %ld\n\033[0m",
			   prev_path.data(), camera->renderCount());
	}
}

void Renderer::checkIfReady()
{
	if (scene == nullptr) TERMINATE("Error: scene is not setup yet.")
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.")
	if (algorithm == nullptr) TERMINATE("Error: algorithm is not setup yet.")

	printf("\033[34m\n----------------------------------------------------------------\n");
	printf("loaded %ld objects, %ld triangle meshes in total.\n",
		   scene->getSingletonCount(), scene->getMeshCount());
	printf("camera viewpoint at %s  orienting towards %s\n",
		   camera->viewpoint().toString().data(), camera->orientation().toString().data());
	printf("algorithm info: %s", algorithm->info().data());
	printf("\n\033[1;34m---------------------- ready to draw -------------------------\n\n\033[0m");
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
	scene->prepare();
	printf("\033[1;36m===== rendering start =====\n\033[0m");

	double since = omp_get_wtime();
	renderFrame();
	auto elapse = lround(omp_get_wtime() - since);

	printf("\033[1;36m\n===== rendering finished in %ld min %ld sec =====\n\033[0m", elapse / 60, elapse % 60);
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
	if (camera == nullptr) TERMINATE("Error: camera is not setup yet.")
	camera->writePPM(save_path);
	printf("\033[32mimage written to \"%s\" \n\033[0m", save_path.data());
}

void Renderer::maybeSaveProgress(size_t cur_epoch) const
{
	if (save_step == 0 || cur_epoch % save_step > 0) return;
	camera->writePPM(save_path);
	printf("\033[32m  progress saved to \"%s\"\n\033[0m", save_path.data());
}

// !!
void Renderer::render()
{
	// without progressbar, faster version
	auto since = omp_get_wtime();
	algorithm->start(
			n_epoch,
			[this, since](size_t cur_epoch) {
				auto eta = lround((omp_get_wtime() - since) * (n_epoch - cur_epoch) / cur_epoch);
				barInfo("\r=== epoch %ld / %ld ===  eta: %ld min %ld sec", cur_epoch + 1, n_epoch, eta / 60, eta % 60)
			},
			[](size_t) {},
			[this](size_t cur_epoch) { maybeSaveProgress(cur_epoch); }
	);
}

// !!
void Renderer::renderVerbose()
{
	// with progressbar
	auto since = omp_get_wtime();
	algorithm->start(
			n_epoch,
			// callbeck before eppch
			[this, since](size_t cur_epoch) {
				auto eta = lround((omp_get_wtime() - since) * (n_epoch - cur_epoch) / cur_epoch);
				barInfo("=== epoch %ld / %ld ===  eta: %ld min %ld sec\n", cur_epoch + 1, n_epoch, eta / 60, eta % 60)
			},
			// callback inside epoch
			[this](size_t cur_j) {
				if (cur_j % verbose_step == 0) barInfo("\r %.1f %%", 1.f * cur_j / camera->height * 100)
			},
			// callback after epoch
			[this](size_t cur_epoch) {
				barInfo("\n");
				maybeSaveProgress(cur_epoch);
			}
	);
}
