//
// Created by James on 2019/4/2.
//

#include "Camera.h"

Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_, size_t height_, double pixel_size_) :
		img(width_, height_), render_cnt(0),
		pos(pos_), ea(euler_angles_),
		ex(Dir(Dir::X_AXIS).rotate(ea)), ey(Dir(Dir::Y_AXIS).rotate(ea)), ez(Dir(Dir::Z_AXIS).rotate(ea)),
		w_2(width_ * 1.0 / 2), h_2(height_ * 1.0 / 2),
		width(width_), height(height_),
		pixel_size(pixel_size_)
{
}

void Camera::readPPM(const String &ppm_path)
{
	size_t width_, height_;
	img.readPPM(ppm_path, width_, height_, render_cnt);
}

void Camera::writePPM(const String &ppm_path) const
{
	img.writePPM(ppm_path, render_cnt);
}

