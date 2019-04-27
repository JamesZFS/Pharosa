//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"
#include <fstream>


Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_, size_t height_, double pixel_size_) :
		cur_i(0), cur_j(0), cur_rank(0),
		pos(pos_), ex(Dir::X_AXIS), ey(Dir::Y_AXIS), ez(Dir::Z_AXIS), ea(euler_angles_),
		width(width_), height(height_), size(width_ * height_), w_2(width_ >> 1), h_2(height_ >> 1),
		pixel_size(pixel_size_)
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

void Camera::readPPM(const String &ppm_path, const String &cpt_path)
{
	std::fstream fppm, fcpt;
	fppm.open(ppm_path, std::ios::in);
	fcpt.open(cpt_path, std::ios::in);	// checkpoint

	if (!fppm.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, reading stopped.", ppm_path.data())
	if (!fcpt.is_open()) TERMINATE("Error: cpt_path \"%s\" cannot be opened, reading stopped.", cpt_path.data())

	String format;
	size_t a, b, c, cpt_cnt;	// prev render cnt
	fppm >> format >> a >> b >> c;
	assert(format == "P3");
	assert(a == width);
	assert(b == height);
	assert(c == 255);
	for (size_t i = 0; i < size; ++i) {
		fppm >> a >> b >> c;    // 0 - 255
		fcpt >> cpt_cnt;
		img[i].r += Funcs::inverseGammaCorrection(a) * cpt_cnt;
		img[i].g += Funcs::inverseGammaCorrection(b) * cpt_cnt;
		img[i].b += Funcs::inverseGammaCorrection(c) * cpt_cnt;
		render_cnt[i] += cpt_cnt;
	}
	fppm.close();
	fcpt.close();
}

void Camera::writePPM(const String &ppm_path, const String &cpt_path) const
{
	std::ofstream fppm, fcpt;
	fppm.open(ppm_path, std::ios::out | std::ios::trunc);
	fcpt.open(cpt_path, std::ios::out | std::ios::trunc);

	if (!fppm.is_open()) TERMINATE("Error: ppm_path \"%s\" cannot be opened, writing stopped.", ppm_path.data())
	if (!fcpt.is_open()) TERMINATE("Error: cpt_path \"%s\" cannot be opened, writing stopped.", cpt_path.data())

	Buffer buffer;
	// write head
	sprintf(buffer, "P3 %ld %ld \n%d \n", width, height, 255);
	fppm << buffer;
	// write body
	for (size_t i = 0; i < size; ++i) {
		const Color p = img[i] / render_cnt[i];
		sprintf(buffer, "%ld %ld %ld ",
				Funcs::gammaCorrection(p.r),
				Funcs::gammaCorrection(p.g),
				Funcs::gammaCorrection(p.b));
		fppm << buffer;
		fcpt << render_cnt[i] << " ";
	}
	fppm.close();
	fcpt.close();
}

void Camera::rotate(const ElAg &euler_angles)
{
	(ex = Dir::X_AXIS).rotate(euler_angles).rotate(ea);
	(ey = Dir::Y_AXIS).rotate(euler_angles).rotate(ea);
	(ez = Dir::Z_AXIS).rotate(euler_angles).rotate(ea);
	ea = ez.getEulerAngles();
}
