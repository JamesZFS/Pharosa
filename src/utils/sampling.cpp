//
// Created by James on 2019/5/16.
//

#include "sampling.h"

namespace Sampling
{
	Vec3f uniformOnHemisphere(const Vec2f &xi)
	{
		auto r = sqrtf(max2(0.f, 1 - xi.x * xi.x));
		auto phi = 2 * M_PIF * xi.y;
		return {r * cosf(phi), r * sinf(phi), xi.x};
	}

	Vec3f cosineOnHemisphere(const Vec2f &xi)
	{
//		auto r = sqrtf(xi.x);
//		auto phi = 2 * M_PIF * xi.y;
//		return {r * cosf(phi), r * sinf(phi), sqrtf(max2(0., 1 - xi.x))};
		auto p = concentricOnDisk(xi);
		return {p.x, p.y, sqrtf(max2(0.f, 1 - p.x * p.x - p.y * p.y))};
	}

	Vec2f concentricOnDisk(const Vec2f &xi)
	{
		Vec2f xi_offset = {2 * xi.x - 1, 2 * xi.y - 1};	// map to [-1, 1]^2
		if (xi_offset.x == 0 && xi_offset.y == 0) return {0, 0};
		real phi, r;
		if (fabsf(xi_offset.x) > fabsf(xi_offset.y)) {
			r = xi_offset.x;
			phi = xi_offset.y / xi_offset.x * M_PI_4F;
		}
		else {
			r = xi_offset.y;
			phi = M_PI_2F - xi_offset.x / xi_offset.y * M_PI_4F;
		}
		return {r * cosf(phi), r * sinf(phi)};
	}
}
