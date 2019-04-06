//
// Created by James on 2019/4/2.
//

#include "Illuminator.h"

unsigned int Illuminator::N_SUBPIXEL = 10;

Illuminator::Illuminator(Stage &stage_, Cameras::Camera &camera_) : stage(stage_), camera(camera_)
{
	is_edge = new bool[camera.size];
}

#define SUB_D1 0.45
#define SUB_D2 0.05

void Illuminator::render(unsigned int n_epoch, unsigned int prev_epoch, const String &checkpoint_dir)
{
	// without progressbar, fast version
	bool checkpoint = (checkpoint_dir.length() > 0);    // whether to save checkpoints
	unsigned int tot_epoch = n_epoch + prev_epoch;

	computeEdgePixels();
	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; ++epoch) {	// for samples
		debug("\n=== epoch %d / %d ===\n", epoch + 1, tot_epoch);
		fflush(stdout);
		#pragma omp parallel for schedule(dynamic, 1)
		for (unsigned int j = 0; j < camera.height; ++j) {				// for each pixel
			for (unsigned int i = 0, rank = j * camera.width; i < camera.width; ++i, ++rank) {
				if (is_edge[rank]) {									// MSAA - for 4 subpixels
					Color color;
					color += radiance(camera.shootRayAt(i - SUB_D1, j - SUB_D2, 0));
					color += radiance(camera.shootRayAt(i - SUB_D2, j + SUB_D1, 0));
					color += radiance(camera.shootRayAt(i + SUB_D2, j - SUB_D1, 0));
					color += radiance(camera.shootRayAt(i + SUB_D1, j + SUB_D2, 0));
					color /= 4;
					camera.render(rank, color);
				}
				else {
					camera.render(rank, radiance(camera.shootRayAt(i, j, 0.5)));	// just rand normal - 0.5
				}
			}
		}
		if (checkpoint) {    // save checkpoints
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %d.ppm", checkpoint_dir.data(), epoch + 1);
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

//	computeEdgePixels();
	for (unsigned int epoch = prev_epoch; epoch < tot_epoch; epoch += N_SUBPIXEL) {
		debug("\n=== epoch %d - %d / %d ===\n", epoch + 1, epoch + N_SUBPIXEL, tot_epoch);
		fflush(stdout);
		camera.resetProgress();
		while (!camera.finishedVerbose(verbose_step)) {    // slight difference here
			for (unsigned int k = 0; k < N_SUBPIXEL; ++k) {
				Ray &&ray = camera.shootRay();
				camera.render(radiance(ray));
			}
			camera.updateProgress();
		}
		if (checkpoint) {    // save checkpoints
			Buffer out_path;
			sprintf(out_path, "%s/epoch - %d.ppm", checkpoint_dir.data(), epoch + N_SUBPIXEL);
			camera.writePPM(out_path);
		}
	}
	debug("\n");
}

Illuminator::~Illuminator()
{
	delete[] is_edge;
}
void Illuminator::computeEdgePixels()
{
	#pragma omp parallel for schedule(dynamic, 1)		// OpenMP
	for (unsigned int j = 0; j < camera.height; ++j) {
		for (unsigned int i = 0, rank = j * camera.width; i < camera.width; ++i, ++rank) {
			// four sub rays
			const Scenes::Object
					*hit0 = stage.hitOf(camera.shootRayAt(i - SUB_D1, j - SUB_D2, 0)),
					*hit1 = stage.hitOf(camera.shootRayAt(i - SUB_D2, j + SUB_D1, 0)),
					*hit2 = stage.hitOf(camera.shootRayAt(i + SUB_D2, j - SUB_D1, 0)),
					*hit3 = stage.hitOf(camera.shootRayAt(i + SUB_D1, j + SUB_D2, 0));
			is_edge[rank] = !(hit0 == hit1 && hit0 == hit2 && hit0 == hit3);	// if hit different objs, mark true
		}
	}
	// debugging todo
	std::ofstream fout("is_edge.ppm");
	if (!fout.is_open()) {
		warn("not open is_edge");
		exit(1);
	}
	fout << "P3 " << camera.width << " " << camera.height << "\n255\n";
	for (unsigned int rank = 0; rank < camera.size; ++rank) {
		fout << (is_edge[rank] ? "0 0 0 " : "255 255 255 ");
	}
	fout.close();
}

#undef SUB_D1
#undef SUB_D2

Color Illuminator::radiance(const Ray &ray) const
{
	return radiance(ray, 0);
}
