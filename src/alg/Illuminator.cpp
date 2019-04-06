//
// Created by James on 2019/4/2.
//

#include "Illuminator.h"


Illuminator::Illuminator(Stage &stage_, Cameras::Camera &camera_) : stage(stage_), camera(camera_)
{
}

#define N_SUBPIXEL 10

void Illuminator::render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir)
{
	// without progressbar, fast version
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;

	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; epoch += N_SUBPIXEL) {
		debug("\n=== epoch %d - %d / %d ===\n", epoch + 1, epoch + N_SUBPIXEL, tot_epoch);
		fflush(stdout);
//		#pragma omp parallel for schedule(static, camera.size / N_SUBPIXEL)  // OpenMP
		#pragma omp parallel for schedule(dynamic, 1)  // OpenMP
		for (unsigned int rank = 0; rank < camera.size; ++rank) {
			for (unsigned int k = 0; k < N_SUBPIXEL; ++k) {    // repeat 4 subpixels
				Ray &&ray = camera.shootRay(rank);
				camera.render(rank, radiance(ray, 0));
			}
		}
		if (checkpoint) {    // save checkpoints
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %d.ppm", checkpoint_dir.data(), epoch);
			camera.writePPM(out_path);
		}
	}
}

void Illuminator::renderVerbose(unsigned int n_epoch, unsigned int prev_epoch,
								unsigned int verbose_step, const String &checkpoint_dir)
{
	// with progressbar
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;

	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; epoch += N_SUBPIXEL) {
		debug("\n=== epoch %d - %d / %d ===\n", epoch + 1, epoch + N_SUBPIXEL, tot_epoch);
		fflush(stdout);
		camera.resetProgress();
		while (!camera.finishedVerbose(verbose_step)) {    // slight difference here
			for (unsigned int k = 0; k < N_SUBPIXEL; ++k) {
				Ray &&ray = camera.shootRay();
				camera.render(radiance(ray, 0));
			}
			camera.updateProgress();
		}
		if (checkpoint) {    // save checkpoints
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %d.ppm", checkpoint_dir.data(), epoch);
			camera.writePPM(out_path);
		}
	}
	debug("\n");
}

#undef N_SUBPIXEL
