//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_CAMERA_H
#define PHAROSA_CAMERA_H

#include "../lib.h"
#include "../Ray.hpp"
#include "../Vec.hpp"

// standard camera api, base class
class Camera
{
private:
	Color *img;            // rendered image, float [0, 1]
	unsigned int *render_cnt;    // rendering times for each pixel
	unsigned int cur_i, cur_j, cur_rank;    // current pixel to render, using screen coordinate sys

protected:
	Pos pos;        // position of the viewpoint
	Dir ex, ey, ez;    // orthogonal basis vectors, where ex ^ ey = ez, ez is the direction the cam faces

public:
	const unsigned int width, height, size;    // image width and height, n_pixel
	const double w_2, h_2;            // width / 2, height / 2

	Camera(const Pos &pos_, const ElAg &euler_angles_, unsigned int width_ = 1024, unsigned int height_ = 768);

	virtual ~Camera();

	// getter:
	inline const Color &pixelAt(unsigned int i, unsigned int j) const;    // img

	inline const Pos &viewpoint() const;    // pos

	inline const Dir &orientation() const;    // ez

	// setter:
	inline void render(const Color &color);    // render incrementally

	inline void render(unsigned int rank, const Color &color);    // render incrementally at given rank

	inline void renderAt(unsigned int i, unsigned int j, const Color &color);

	// iterators:
	inline bool finished() const;    // check whether the rendering progress is finished

	inline bool finishedVerbose(unsigned int n_step) const; // as above, with a progressbar displayed every n_step

	inline void updateProgress();    // current pixel rank++

	inline void resetProgress();   // reset shooting progress

	// io:
	// load from previous rendered ppm file to continue, which was rendered n_epoch times
	void readPPM(String prev_path, unsigned int prev_epoch);

	// output image into ppm format
	void writePPM(String out_path) const;

	inline Ray shootRay() const;    // shoot a ray iteratively. will stop when all pixels are traversed

	inline Ray shootRay(unsigned int rank) const;    // shoot a ray at a given pixel rank

	// interface:
	virtual Ray shootRayAt(unsigned int i, unsigned int j) const = 0;
};

#include "Camera.cpp"

#endif //PHAROSA_CAMERA_H
