//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"

Camera::Camera(const Pos &pos_, const Dir &dir_, unsigned int width_, unsigned int height_) :
		post_processed(false), pos(pos_), dir(dir_), width(width_), height(height_), size(width_ * height_)
{
	img = new Color[size];
	render_cnt = new unsigned int[size];
	memset(render_cnt, 0, size * sizeof(unsigned int));
}

Camera::~Camera()
{
	delete[] img;
	delete[] render_cnt;
}

#define checkCrd assert(0 <= x && x < width && 0 <= y && y < height)

const Color &Camera::pixelAt(unsigned int x, unsigned int y) const
{
	checkCrd;
	return img[y * width + x];
}

void Camera::renderInc(unsigned int x, unsigned int y, Color color)
{
	checkCrd;
	unsigned int rank = y * width + x;
	img[rank] += color;
	++render_cnt[rank];	// count up
}

#undef checkCrd

const Ray &Camera::shootRay()
{
	return cur_ray;
}

double Camera::progress()
{
	return 0;
}

void Camera::postProcess()
{
	// assert: progress == 1
	for (unsigned int i = 0; i < size; ++i) {
		img[i] /= render_cnt[i];
	}
	post_processed = true;
}

void Camera::writePPM(String out_path) const
{
	if (!post_processed) {
		warn("warning: not post_processed yet.");
	}
	if (!Funcs::endsWith(out_path, ".ppm")) {
		out_path += ".ppm";
	}
	std::ofstream f;
	f.open(out_path, std::ios::out | std::ios::trunc);

	char buffer[250];
	if (f.is_open()) {
		// write head
		sprintf(buffer, "P3 %d %d\n%d\n", width, height, 255);
		f << buffer;
		// write body
		for (unsigned int i = 0; i < size; ++i) {
			sprintf(buffer, "%d %d %d ", Funcs::toUchar(img[i].x), Funcs::toUchar(img[i].y), Funcs::toUchar(img[i].z));
			f << buffer;
		}
	}
	else {
		sprintf(buffer, "IO error, out_path \"%s\" cannot be opened, writing stopped.", out_path.data());
		warn(buffer);
	}
}
