//
// Created by James on 2019/5/16.
//

#include "sampling.h"

namespace Sampling
{
	Vec3f uniformOnSphere(const Vec2f &xi)
	{
		auto cos_theta = 1 - 2 * xi[0];  // [-1, 1]
		auto sin_theta = sqrtf(max2(0.f, 1 - cos_theta * cos_theta));
		auto phi = 2 * M_PIF * xi[1];
		return {sin_theta * cosf(phi), sin_theta * sinf(phi), cos_theta};
	}

	Vec3f uniformOnHemisphere(const Vec2f &xi)
	{
		auto sin_theta = sqrtf(max2(0.f, 1 - xi[0] * xi[0]));
		auto phi = 2 * M_PIF * xi[1];
		return {sin_theta * cosf(phi), sin_theta * sinf(phi), xi[0]};
	}

	Vec3f cosineOnHemisphere(const Vec2f &xi)
	{
		auto p = uniformOnDisk(xi);
		return {p.x, p.y, sqrtf(max2(0.f, 1 - p.x * p.x - p.y * p.y))};
	}

	Vec2f uniformOnDisk(const Vec2f &xi)
	{
		Vec2f xi_offset = {2 * xi[0] - 1, 2 * xi[1] - 1};	// map to [-1, 1]^2
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

	Vec3f uniformOnSphereCap(real cos_theta_max, const Vec2f &xi)
	{
		auto cos_theta = 1 - xi[0] + xi[0] * cos_theta_max;
		auto sin_theta = sqrtf(max2(0.f, 1 - cos_theta * cos_theta));
		auto phi = 2 * M_PIF * xi[1];
		return {sin_theta * cosf(phi), sin_theta * sinf(phi), cos_theta};
	}

	Vec3f uniformOnTriangle(const Pos &A, const Pos &B, const Pos &C, const Vec2f &xi)
	{
		real sxi0 = sqrtf(xi[0]);
		return A * ((1 - xi[1]) * sxi0) + B * (1 - sxi0) + C * (xi[1] * sxi0);
	}
}
