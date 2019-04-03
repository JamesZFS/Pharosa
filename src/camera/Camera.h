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

protected:
	Pos pos;        // camera position
	unsigned int width, height, size;    // image width and height, n_pixel
	Pos ex, ey, ez;    // orthogonal basis vectors, where ex ^ ey = ez, ez is the direction the cam faces

	unsigned int cur_i, cur_j, cur_rank;    // current pixel to render, using screen coordinate sys
	Ray cur_ray;    // current ray to shoot

public:
	Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	~Camera();

	// getter:
	inline const Color &pixelAt(unsigned int i, unsigned int j) const;

	// setter:
	inline void renderInc(const Color &color);    // render incrementally

	// iterators:
	inline bool finished() const;    // check whether the rendering progress is finished

	inline bool finishedDisplay(unsigned int n_step = 50) const; // as above, with a progressbar displayed every n_step

	inline void updateProgress();    // current pixel rank++

	inline void resetProgress();   // reset shooting progress

	// io:
	void writePPM(String out_path) const;        // output image into ppm format

	// interface:
	virtual const Ray &shootRay() = 0;    // shoot a ray iteratively. will stop when all pixels are traversed

	// camera constants
	static const double PIXEL_SIZE, CAMERA_FOCUS;
};

const double Camera::PIXEL_SIZE = 1.0, Camera::CAMERA_FOCUS = 150.0;

#include "Camera.cpp"

#endif //RAYTRACKER_CAMERA_H
