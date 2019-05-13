//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"
#include <fstream>
#include <sstream>


Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_, size_t height_, double pixel_size_) :
		pos(pos_), ex(Dir::X_AXIS), ey(Dir::Y_AXIS), ez(Dir::Z_AXIS), ea(euler_angles_),
		width(width_), height(height_), size(width_ * height_), w_2(width_ >> 1), h_2(height_ >> 1),
		pixel_size(pixel_size_)
{
	img = new Color[size];
	render_cnt = 0;
	// calculate basis vectors
	ex.rotate(ea);
	ey.rotate(ea);
	ez.rotate(ea);
}

Camera::~Camera()
{
	delete[] img;
}

void Camera::readPPM(const String &ppm_path)
{
	std::fstream fin;
	fin.open(ppm_path, std::ios::in);

	if (!fin.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, reading stopped.", ppm_path.data())

	String format, buffer_str;
	size_t a, b, c;    // prev render cnt
	Buffer buffer;

	fin >> format >> a >> b >> c;
	fin.getline(buffer, 256);
	buffer_str.assign(buffer);
	if (buffer_str.find("render_cnt") != String::npos) {
		std::stringstream stream(buffer);
		do stream >> buffer_str; while (buffer_str != "render_cnt");
		stream >> buffer_str;
		assert(buffer_str == "=");
		stream >> render_cnt;
	}
	else {
		render_cnt = 0;
	}
	assert(format == "P3");
	assert(a == width);
	assert(b == height);
	assert(c == 255);
	for (size_t i = 0; i < size; ++i) {
		fin >> a >> b >> c;    // 0 - 255
		img[i].r += Funcs::inverseGammaCorrection(a) * render_cnt;
		img[i].g += Funcs::inverseGammaCorrection(b) * render_cnt;
		img[i].b += Funcs::inverseGammaCorrection(c) * render_cnt;
	}
	fin.close();
}

void Camera::writePPM(const String &ppm_path) const
{
	std::ofstream fout;
	fout.open(ppm_path, std::ios::out | std::ios::trunc);

	if (!fout.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, writing stopped.", ppm_path.data())

	// write head
	fout << "P3 " << width << " " << height << "\n255 # render_cnt = " << render_cnt << "\n";
	// write body
	for (size_t i = 0; i < size; ++i) {
		const Color p = img[i] / render_cnt;
		fout <<
			 Funcs::gammaCorrection(p.r) << " " <<
			 Funcs::gammaCorrection(p.g) << " " <<
			 Funcs::gammaCorrection(p.b) << " ";
	}
	fout.close();
}
