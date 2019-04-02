//
// Created by James on 2019/4/2.
//

#include "Camera.h"
#include "../utils/funcs.hpp"

Camera::Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_, unsigned int height_) :
		pos(pos_), width(width_), height(height_), size(width_ * height_),
		ex(1, 0, 0), ey(0, 1, 0), ez(0, 0, 1)
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
	++render_cnt[rank];    // rendering times count up
}

#undef checkCrd

const Ray &Camera::shootRay()
{
	return cur_ray;
}

double Camera::progress() const
{
	return 0;
}

void Camera::resetProgress()
{

}

void Camera::writePPM(String out_path) const
{
	if (!Funcs::endsWith(out_path, ".ppm")) {
		out_path += ".ppm";
	}
	std::ofstream f;
	f.open(out_path, std::ios::out | std::ios::trunc);

	char buffer[250];
	if (f.is_open()) {
		// write head
		sprintf(buffer, "P3 %d %d \n%d \n", width, height, 255);
		f << buffer;
		// write body
		for (unsigned int i = 0; i < size; ++i) {
			const Color p = img[i] / render_cnt[i];
			sprintf(buffer, "%d %d %d ", Funcs::toUchar(p.x), Funcs::toUchar(p.y), Funcs::toUchar(p.z));
			f << buffer;
		}
	}
	else {
		sprintf(buffer, "IO error, out_path \"%s\" cannot be opened, writing stopped.", out_path.data());
		warn(buffer);
	}
	f.close();
}
