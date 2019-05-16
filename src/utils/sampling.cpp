//
// Created by James on 2019/5/16.
//

#include "sampling.h"

namespace Sampling
{
	Vec3f uniformOnHemisphere(const Vec2f &xi)
	{
		auto r = sqrt(max2(0., 1 - xi.x * xi.x));
		auto phi = 2 * M_PI * xi.y;
		return {r * cos(phi), r * sin(phi), xi.x};
	}

	Vec3f cosineOnHemisphere(const Vec2f &xi)
	{
//		auto r = sqrt(xi.x);
//		auto phi = 2 * M_PI * xi.y;
//		return {r * cos(phi), r * sin(phi), sqrt(max2(0., 1 - xi.x))};
		auto p = concentricOnDisk(xi);
		return {p.x, p.y, sqrt(max2(0., 1 - p.x * p.x - p.y * p.y))};
	}

	Vec2f concentricOnDisk(const Vec2f &xi)
	{
		Vec2f xi_offset = {2 * xi.x - 1, 2 * xi.y - 1};	// map to [-1, 1]^2
		if (xi_offset.x == 0 && xi_offset.y == 0) return {0, 0};
		double phi, r;
		if (fabs(xi_offset.x) > fabs(xi_offset.y)) {
			r = xi_offset.x;
			phi = xi_offset.y / xi_offset.x * M_PI_4;
		}
		else {
			r = xi_offset.y;
			phi = M_PI_2 - xi_offset.x / xi_offset.y * M_PI_4;
		}
		return {r * cos(phi), r * sin(phi)};
	}
}
