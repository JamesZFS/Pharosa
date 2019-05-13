//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_CAMERA_H
#define PHAROSA_CAMERA_H

#include "../defs.h"
#include "../core/Ray.hpp"
#include "../core/Vec.h"
#include "../core/Image.h"

// standard camera api, base class
class Camera
{
private:
	Image img;
	size_t render_cnt;     // rendering times

protected:
	Pos pos;        // position of the viewpoint
	ElAg ea;        // euler angles cache
	Dir ex, ey, ez;    // orthogonal basis vectors, where ex ^ ey = ez, ez is the direction the cam faces
	const double w_2, h_2;            // width / 2, height / 2

public:
	const size_t width, height;		// image size
	const double pixel_size;		// physical size of a pixel

	Camera(const Pos &pos_, const ElAg &euler_angles_, size_t width_ = 1024, size_t height_ = 768,
		   double pixel_size_ = 0.1);

	Camera(const Json &json);

	virtual ~Camera() = default;

	// getter:
	inline const Pos &viewpoint() const;    // c

	inline const Dir &orientation() const;    // ez

	inline size_t renderCount() const;

	// setter:
	inline void render(size_t i, size_t j, const Color &color);

	inline void rotate(const ElAg &euler_angles);	// rotate inc

	inline void translate(const Pos &delta);	// move inc

	inline void step();   // ++render_cnt

	// io:
	// load from previous rendered ppm file to continue
	void readPPM(const String &ppm_path);

	// output image into ppm format and checkpoint
	void writePPM(const String &ppm_path) const;

	// interface:
	// shoot ray at (i, j), offset deferring normal dist 0 - sigma
	inline Ray shootRayAt(double i, double j, double sigma) const;

	// shoot precise ray
	virtual Ray shootRayAt(double i, double j) const = 0;

	static Camera *acquire(const Json &json);	// from json
};

// inline methods:
#include "Camera.inl"

#endif //PHAROSA_CAMERA_H
