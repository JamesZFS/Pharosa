//
// Created by James on 2019/5/13.
//

#ifndef PHAROSA_IMAGE_H
#define PHAROSA_IMAGE_H

#include "../Pharosa.h"
#include "Vec.h"

class Image
{
private:
	List2D<Color> data;    // width * height
	size_t width, height;
public:
	Image(const String &ppm_path);    // create from ppm

	Image(size_t width_, size_t height_);    // create an empty image

	void setup(size_t width_, size_t height_);

	inline Color &at(size_t i, size_t j)    // get pixel without index checking
	{ return data[i][j]; }

	const Color &get(double x, double y) const;

	/** read from PPM file
	 * input: ppm_path
	 * output: width_, height_, render_cnt_
	 */
	void readPPM(const String &ppm_path, size_t &width_, size_t &height_, size_t &render_cnt_);

	void writePPM(const String &ppm_path, size_t render_cnt) const;
};


#endif //PHAROSA_IMAGE_H
