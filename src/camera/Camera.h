//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_CAMERA_H
#define PHAROSA_CAMERA_H

#include "../defs.h"
#include "../core/Ray.hpp"
#include "../core/Vec.h"

// standard camera api, base class
class Camera
{
private:
	Color *img;            // rendered image, float [0, 1]
	size_t *render_cnt;    // rendering times for each pixel
	size_t cur_i, cur_j, cur_rank;    // current pixel to render, using screen coordinate sys

protected:
	Pos pos;        // position of the viewpoint
	Dir ex, ey, ez;    // orthogonal basis vectors, where ex ^ ey = ez, ez is the direction the cam faces
	ElAg ea;        // euler angles cache

public:
	const size_t width, height, size;    // image width and height, n_pixel
	const double w_2, h_2;            // width / 2, height / 2
	const double pixel_size;

	Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_ = 1024, size_t height_ = 768,
		   double pixel_size_ = 0.1);

	Camera(const Json &json);

	virtual ~Camera();

	// getter:
	inline const Color &pixelAt(size_t i, size_t j) const;    // img

	inline const Pos &viewpoint() const;    // pos

	inline const Dir &orientation() const;    // ez

	// setter:
	inline void render(const Color &color);    // render incrementally

	inline void render(size_t rank, const Color &color);    // render incrementally at given rank

	inline void renderAt(size_t i, size_t j, const Color &color);

	void rotate(const ElAg &euler_angles);	// rotate inc

	inline void translate(const Pos &delta);	// move inc

	// iterators:
	inline bool finished() const;    // check whether the rendering progress is finished

	inline bool finishedVerbose(size_t n_step) const; // as above, with a progressbar displayed every n_step

	inline void updateProgress();    // current pixel rank++

	inline void resetProgress();   // reset shooting progress

	// io:
	// load from previous rendered ppm file to continue
	void readPPM(const String &in_path);

	// output image into ppm format
	void writePPM(const String &ppm_path, const String &cp_path) const;

	inline Ray shootRay() const;    // shoot a ray iteratively. will stop when all pixels are traversed

	inline Ray shootRay(size_t rank) const;    // shoot a ray at a given pixel rank

	// interface:
	// shoot ray at (i, j), offset deferring normal dist 0 - sigma
	virtual Ray shootRayAt(double i, double j, double sigma) const = 0;

	// shoot precise ray
	virtual Ray shootRayAt(double i, double j) const = 0;

	static Camera *acquire(const Json &json);	// from json
};

// inline methods:
#include "Camera.inl"

#endif //PHAROSA_CAMERA_H
