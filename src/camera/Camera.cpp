//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"

Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_, size_t height_) :
		cur_i(0), cur_j(0), cur_rank(0),
		pos(pos_), ex(Dir::X_AXIS), ey(Dir::Y_AXIS), ez(Dir::Z_AXIS), ea(euler_angles_),
		width(width_), height(height_), size(width_ * height_), w_2(width_ >> 1), h_2(height_ >> 1)
{
	img = new Color[size];
	render_cnt = new size_t[size];
	memset(render_cnt, 0, size * sizeof(size_t));
	// calculate basis vectors
	ex.rotate(ea);
	ey.rotate(ea);
	ez.rotate(ea);
}

Camera::~Camera()
{
	delete[] img;
	delete[] render_cnt;
}

void Camera::readPPM(String prev_path, size_t prev_epoch)
{
	if (prev_epoch == 0) return;
	if (!Funcs::endsWith(prev_path, ".ppm")) {
		prev_path += ".ppm";
	}
	std::fstream fin;
	fin.open(prev_path, std::ios::in);

	Buffer buffer;
	if (!fin.is_open()) {
		sprintf(buffer, "Error: prev_path \"%s\" cannot be opened, reading stopped.", prev_path.data());
		warn(buffer);
		fin.close();
		exit(1);
	}
	String format;
	size_t a, b, c;
	fin >> format >> a >> b >> c;
	assert(format == "P3");
	assert(a == width);
	assert(b == height);
	assert(c == 255);
	for (size_t i = 0; i < size; ++i) {
		fin >> a >> b >> c;    // 0 - 255
		img[i].r += Funcs::inverseGammaCorrection(a) * prev_epoch;
		img[i].g += Funcs::inverseGammaCorrection(b) * prev_epoch;
		img[i].b += Funcs::inverseGammaCorrection(c) * prev_epoch;
		render_cnt[i] += prev_epoch;
	}
}

void Camera::writePPM(String out_path) const
{
	if (!Funcs::endsWith(out_path, ".ppm")) {
		out_path += ".ppm";
	}
	std::ofstream fout;
	fout.open(out_path, std::ios::out | std::ios::trunc);

	Buffer buffer;
	if (!fout.is_open()) {
		sprintf(buffer, "Error: out_path \"%s\" cannot be opened, writing stopped.", out_path.data());
		warn(buffer);
		fout.close();
		exit(1);
	}
	// write head
	sprintf(buffer, "P3 %ld %ld \n%d \n", width, height, 255);
	fout << buffer;
	// write body
	for (size_t i = 0; i < size; ++i) {
		const Color p = img[i] / render_cnt[i];
		sprintf(buffer, "%ld %ld %ld ",
				Funcs::gammaCorrection(p.r),
				Funcs::gammaCorrection(p.g),
				Funcs::gammaCorrection(p.b));
		fout << buffer;
	}
	fout.close();
}


void Camera::rotate(const ElAg &euler_angles)
{
	(ex = Dir::X_AXIS).rotate(euler_angles).rotate(ea);
	(ey = Dir::Y_AXIS).rotate(euler_angles).rotate(ea);
	(ez = Dir::Z_AXIS).rotate(euler_angles).rotate(ea);
	ea = ez.getEulerAngles();
}
