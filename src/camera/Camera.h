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
	Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768,
		   const String &prev_path_ = "", unsigned int n_epoch_ = 1);

	virtual ~Camera();

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
	// load from previous rendered ppm file to continue, which was rendered n_epoch times
	void readPPM(String prev_path, unsigned int n_epoch = 1);

	// output image into ppm format
	void writePPM(String out_path) const;

	// interface:
	virtual const Ray &shootRay() = 0;    // shoot a ray iteratively. will stop when all pixels are traversed

	// camera constants
	static const double PIXEL_SIZE, CAMERA_FOCUS;
};

#include "Camera.cpp"

#endif //RAYTRACKER_CAMERA_H
