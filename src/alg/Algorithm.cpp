//
// Created by James on 2019/4/2.
//

#include "Algorithm.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include <fstream>

Algorithm::Algorithm(Scene &scene_, Camera &camera_) : scene(scene_), camera(camera_), is_edge(new bool[camera.size])
{
}

Algorithm::~Algorithm()
{
	delete[] is_edge;
}

#define SUB_D1 0.45
#define SUB_D2 0.05

void Algorithm::render(size_t n_epoch, size_t prev_epoch, const String &checkpoint_dir)
{
	// without progressbar, fast version
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	size_t tot_epoch = n_epoch + prev_epoch;

	detectEdges();
	for (size_t epoch = prev_epoch; epoch < tot_epoch; ++epoch) {    // for samples
		barInfo("\r=== epoch %ld / %ld ===", epoch + 1, tot_epoch);
		fflush(stdout);

#ifdef __USE_OMP__
#pragma omp parallel for schedule(dynamic, 1)
#endif

		for (size_t j = 0; j < camera.height; ++j) {                // for each pixel
			for (size_t i = 0, rank = j * camera.width; i < camera.width; ++i, ++rank) {
				if (is_edge[rank]) {                                    // MSAA - for 4 subpixels
					Color color;
					color += radiance(camera.shootRayAt(i - SUB_D1, j - SUB_D2));
					color += radiance(camera.shootRayAt(i - SUB_D2, j + SUB_D1));
					color += radiance(camera.shootRayAt(i + SUB_D2, j - SUB_D1));
					color += radiance(camera.shootRayAt(i + SUB_D1, j + SUB_D2));
					color /= 4;
					camera.render(rank, color);
				}
				else {
					camera.render(rank, radiance(camera.shootRayAt(i, j, 0.5)));    // just rand normal - 0.5
				}
			}
		}
		if (checkpoint/* && (epoch - prev_epoch) % 500 == 0*/) {    // save checkpoints every 500 epochs todo
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %ld.ppm", checkpoint_dir.data(), epoch + 1);
			camera.writePPM(out_path);
		}
	}
	printf("\n");
}

void Algorithm::renderVerbose(size_t n_epoch, size_t prev_epoch,
							  size_t verbose_step, const String &checkpoint_dir)
{
	// with progressbar
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	size_t tot_epoch = n_epoch + prev_epoch;

	detectEdges();
	for (size_t epoch = prev_epoch; epoch < tot_epoch; ++epoch) {    // for samples
		printf("\n=== epoch %ld / %ld ===\n", epoch + 1, tot_epoch);
		fflush(stdout);

#ifdef __USE_OMP__
#pragma omp parallel for schedule(dynamic, 1)
#endif

		for (size_t j = 0; j < camera.height; ++j) {                // for each pixel
			if (j % verbose_step == 0) {
				barInfo("\r %.1f %%", j * 100.0 / camera.height);    // progressbar :)
			}
			for (size_t i = 0, rank = j * camera.width; i < camera.width; ++i, ++rank) {
				if (is_edge[rank]) {                                    // MSAA - for 4 subpixels
					Color color;
					color += radiance(camera.shootRayAt(i - SUB_D1, j - SUB_D2));
					color += radiance(camera.shootRayAt(i - SUB_D2, j + SUB_D1));
					color += radiance(camera.shootRayAt(i + SUB_D2, j - SUB_D1));
					color += radiance(camera.shootRayAt(i + SUB_D1, j + SUB_D2));
					color /= 4;
					camera.render(rank, color);
				}
				else {
					camera.render(rank, radiance(camera.shootRayAt(i, j, 0.5)));    // just rand normal - 0.5
				}
			}
		}
		if (checkpoint/* && (epoch - prev_epoch) % 500 == 0*/) {    // save checkpoints every epoch todo
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %ld.ppm", checkpoint_dir.data(), epoch + 1);
			camera.writePPM(out_path);
		}
	}
	printf("\n");
}

void Algorithm::detectEdges()
{
	printf("detecting edges...\n");
	fflush(stdout);
#pragma omp parallel for schedule(dynamic, 1)        // OpenMP
	for (size_t j = 0; j < camera.height; ++j) {
		barInfo("\r %.1f %%", j * 100.0 / camera.height);    // progressbar :)
		for (size_t i = 0, rank = j * camera.width; i < camera.width; ++i, ++rank) {
			// four sub rays
			const Object
					*hit0 = scene.hitOf(camera.shootRayAt(i - SUB_D1, j - SUB_D2)),
					*hit1 = scene.hitOf(camera.shootRayAt(i - SUB_D2, j + SUB_D1)),
					*hit2 = scene.hitOf(camera.shootRayAt(i + SUB_D2, j - SUB_D1)),
					*hit3 = scene.hitOf(camera.shootRayAt(i + SUB_D1, j + SUB_D2));
			is_edge[rank] = !(hit0 == hit1 && hit0 == hit2 && hit0 == hit3);    // if hit different objs, mark as edge
		}
	}
	// todo cancel printf
#ifdef __DEV_STAGE__
	std::ofstream fout("is_edge.ppm");
	if (!fout.is_open()) {
		warn("not open is_edge");
		exit(1);
	}
	fout << "P3 " << camera.width << " " << camera.height << "\n255\n";
	for (size_t rank = 0; rank < camera.size; ++rank) {
		fout << (is_edge[rank] ? "0 0 0 " : "255 255 255 ");
	}
	fout.close();
#endif
}

void Algorithm::report()
{
	debug("Algorithm::report()\n");
}

#undef SUB_D1
#undef SUB_D2
