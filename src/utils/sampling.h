//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_SAMPLING_H
#define PHAROSA_SAMPLING_H

#include "../core/Vec.h"

namespace Sampling
{
	using Vec3f = Vec<double>;
	struct Vec2f
	{
		double x, y;

		Vec2f() : x(0), y(0) {}

		Vec2f(double x_, double y_) : x(x_), y (y_) {}
	};

	Vec3f uniformOnHemisphere(const Vec2f &xi);

	Vec3f cosineOnHemisphere(const Vec2f &xi);

	Vec2f concentricOnDisk(const Vec2f &xi);
};

#endif //PHAROSA_SAMPLING_H
