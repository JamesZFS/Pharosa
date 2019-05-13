//
// Created by James on 2019/5/13.
//

#include "Image.h"
#include <fstream>
#include <sstream>
#include "../utils/funcs.hpp"

Image::Image(const String &ppm_path)
{
	size_t width_, height_, render_cnt_;
	readPPM(ppm_path, width_, height_, render_cnt_);
}

Image::Image(size_t width_, size_t height_)
{
	setup(width_, height_);
}

void Image::writePPM(const String &ppm_path, size_t render_cnt) const
{
	std::ofstream fout;
	fout.open(ppm_path, std::ios::out | std::ios::trunc);

	if (!fout.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, writing stopped.", ppm_path.data())

	// write head
	fout << "P3 " << width << " " << height << "\n255 # render_cnt = " << render_cnt << "\n";
	// write body
	for (size_t j = 0; j < height; ++j) {
		for (size_t i = 0; i < width; ++i) {
			const Color p = data[i][j] / render_cnt;
			fout <<
				 Funcs::gammaCorrection(p.r) << " " <<
				 Funcs::gammaCorrection(p.g) << " " <<
				 Funcs::gammaCorrection(p.b) << " ";
		}
	}
	fout.close();
}

void Image::readPPM(const String &ppm_path, size_t &width_, size_t &height_, size_t &render_cnt_)
{
	std::fstream fin;
	fin.open(ppm_path, std::ios::in);

	if (!fin.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, reading stopped.", ppm_path.data())

	String format, buffer_str;
	Buffer buffer;
	int max_color;

	fin >> format >> width_ >> height_ >> max_color;
	assert(format == "P3");
	assert(max_color == 255);
	// read render_cnt
	fin.getline(buffer, 256);
	buffer_str.assign(buffer);
	if (buffer_str.find("render_cnt") != String::npos) {
		std::stringstream stream(buffer);
		do stream >> buffer_str; while (buffer_str != "render_cnt");
		stream >> buffer_str;
		assert(buffer_str == "=");
		stream >> render_cnt_;
	}
	else {
		render_cnt_ = 0;
	}
	if (data.empty()) {    // size unassigned
		setup(width_, height_);
	}
	else {        // size assigned
		assert(width_ == width && height_ == height);
	}
	// read body
	size_t r, g, b;
	for (size_t j = 0; j < height; ++j) {
		for (size_t i = 0; i < width; ++i) {
			fin >> r >> g >> b;    // 0 - 255
			data[i][j].r += Funcs::inverseGammaCorrection(r) * render_cnt_;
			data[i][j].g += Funcs::inverseGammaCorrection(g) * render_cnt_;
			data[i][j].b += Funcs::inverseGammaCorrection(b) * render_cnt_;
		}
	}
	fin.close();
}

void Image::setup(size_t width_, size_t height_)
{
	data.clear();
	data.resize(width_, List<Color>(height_, Color::BLACK));
	width = width_, height = height_;
}
