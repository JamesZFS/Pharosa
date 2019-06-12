//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_SAMPLING_H
#define PHAROSA_SAMPLING_H

#include "../core/Vec.h"
#include "../geometric/Triangle.h"

namespace Sampling
{
	Vec3f uniformOnSphere(const Vec2f &xi);

	Vec3f uniformOnHemisphere(const Vec2f &xi);

	Vec3f cosineOnHemisphere(const Vec2f &xi);

	Vec2f uniformOnDisk(const Vec2f &xi);

	Vec3f uniformOnSphereCap(real cos_theta_max, const Vec2f &xi);

	Vec3f uniformOnTriangle(const Triangle &t, const Vec2f &xi);
};

#endif //PHAROSA_SAMPLING_H
