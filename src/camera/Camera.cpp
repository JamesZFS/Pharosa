//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"

Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_) :
		pos(pos_), width(width_), height(height_), size(width_ * height_),
		ex(1, 0, 0), ey(0, 1, 0), ez(0, 0, 1),
		cur_i(0), cur_j(0), cur_rank(0)
{
	img = new Color[size];
	render_cnt = new unsigned int[size];
	memset(render_cnt, 0, size * sizeof(unsigned int));
	// calculate basis vectors
	ex.rotate(euler_angles_).unitize();
	ey.rotate(euler_angles_).unitize();
	ez.rotate(euler_angles_).unitize();
}

Camera::~Camera()
{
	delete[] img;
	delete[] render_cnt;
}

const Color &Camera::pixelAt(unsigned int i, unsigned int j) const
{
	if (0 <= i && i < width && 0 <= j && j < height) {
		return img[j * width + i];
	}
	else {
		warn("Warning: got invalid pixel rank in Camera::pixelAt.\n");
	}
}

void Camera::renderInc(const Color &color)
{
	img[cur_rank] += color;
	++render_cnt[cur_rank];    // counts up rendering time of current pixel
}

bool Camera::finished() const
{
	return (cur_rank >= size);
}

bool Camera::finishedDisplay(unsigned int n_step) const
{
	if (cur_rank % n_step == 0) {
		debug("\r\tprogress:  %.1f %%", cur_rank * 100.0 / size);
	}
	return (cur_rank >= size);
}

void Camera::updateProgress()
{
	++cur_rank;
	if (++cur_i == width) {        // first ++i then ++j
		cur_i = 0;
		if (++cur_j > height) {
			warn("Warning: pixel index overflows.\n");
			resetProgress();
		}
	}
}

void Camera::resetProgress()
{
	cur_i = cur_j = cur_rank = 0;
}

void Camera::writePPM(String out_path) const
{
	if (!Funcs::endsWith(out_path, ".ppm")) {
		out_path += ".ppm";
	}
	std::ofstream fout;
	fout.open(out_path, std::ios::out | std::ios::trunc);

	char buffer[250];
	if (fout.is_open()) {
		// write head
		sprintf(buffer, "P3 %d %d \n%d \n", width, height, 255);
		fout << buffer;
		// write body
		for (unsigned int i = 0; i < size; ++i) {
			const Color p = img[i] / render_cnt[i];
			sprintf(buffer, "%d %d %d ", Funcs::toUchar(p.x), Funcs::toUchar(p.y), Funcs::toUchar(p.z));
			fout << buffer;
		}
	}
	else {
		sprintf(buffer, "IO Error: out_path \"%s\" cannot be opened, writing stopped.", out_path.data());
		warn(buffer);
	}
	fout.close();
}
