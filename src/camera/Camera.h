//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_CAMERA_H
#define RAYTRACKER_CAMERA_H

#include "../lib.h"
#include "../Ray.hpp"
#include "../Vec.hpp"

// standard camera api, base class
class Camera
{
private:
	Color *img;            // rendered image, float [0, 1]
	unsigned int *render_cnt;    // rendering times for each pixel

public:
	Pos pos;        // camera position
	unsigned int width, height, size;    // image width and height, n_pixel

	Pos ex, ey, ez;    // orthogonal basis vectors, where ex ^ ey = ez, ez is the direction the cam faces

	Ray cur_ray;    // current ray to shoot

public:
	Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	~Camera();

	inline const Color &pixelAt(unsigned int x, unsigned int y) const;    // getter

	inline void renderInc(unsigned int x, unsigned int y, Color color);    // render incrementally

	// interfaces:
	virtual const Ray &shootRay();    // shoot a ray iteratively. will stop when all pixels are traversed

	virtual double progress();    // how far have the shooting been, real number [0, 1]

	virtual void resetProgress();   // reset shooting progress

	void writePPM(String out_path) const;        // output image into ppm format

	// camera constants
	static const double PIXEL_SIZE;
	static const double CAMERA_FOCUS;
};

const double Camera::PIXEL_SIZE = 1.0;
const double Camera::CAMERA_FOCUS = 150.0;

#include "Camera.cpp"

#endif //RAYTRACKER_CAMERA_H
