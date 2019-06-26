//
// Created by James on 2019/5/16.
//

#include "BezierRev.h"
#include "../utils/funcs.hpp"

#include <algorithm>

BezierRev::BezierRev(const List<Arr<real, 2>> &ctrl_pts, real scale) : PolyRev()
{
	auto n = (unsigned char) (ctrl_pts.size() - 1);
	for (unsigned char i = 0; i <= n; ++i) {
		auto Bi = Polynomial::binomial(0, 1, i) * Polynomial::binomial(1, -1, n - i) * Funcs::binomial(n, i);
		phi += Bi * (ctrl_pts[i][0] * scale);
		psi += Bi * (ctrl_pts[i][1] * scale);
	}
	psi_2 = psi * psi;
	// find bounding of ctrl_pts:
	real xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = ctrl_pts[0][0] * scale;
	ymax = ctrl_pts[0][1] * scale;
	for (auto i = 1; i <= n; ++i) {
		auto &pt = ctrl_pts[i];
		xmin = min2(xmin, pt[0] * scale);
		xmax = max2(xmax, pt[0] * scale);
		ymax = max2(ymax, pt[1] * scale);
	}
	zmax = ymax;
	zmin = ymin = -ymax;
	bound_pts.reserve(8);
	bound_pts.emplace_back(xmin, ymin, zmin);
	bound_pts.emplace_back(xmin, ymin, zmax);
	bound_pts.emplace_back(xmin, ymax, zmin);
	bound_pts.emplace_back(xmax, ymin, zmin);
	bound_pts.emplace_back(xmin, ymax, zmax);
	bound_pts.emplace_back(xmax, ymin, zmax);
	bound_pts.emplace_back(xmax, ymax, zmin);
	bound_pts.emplace_back(xmax, ymax, zmax);    // a conservative upper bound_pts
	bbox = BoundingBox(bound_pts.cbegin(), bound_pts.cend());
	bbox.extendMargin(EPS);
	c = Pos((xmin + xmax) / 2, 0, 0);
}

void BezierRev::report() const
{
	printf("<Bezier Revolution> bounding box:\n");
	for (auto &pos : bound_pts) {
		pos.report(true);
	}
	printf("center: ");
	c.report(true);
	PolyRev::report();
}

void BezierRev::applyTransform(const TransMat &mat_)
{
	PolyRev::applyTransform(mat_);
	for (auto &pos : bound_pts) {
		pos = mat * pos;
	}
	bbox = BoundingBox(bound_pts.cbegin(), bound_pts.cend());
	bbox.extendMargin(EPS);
	c = mat * c;
}

real BezierRev::xMin() const
{
	return bbox.getLowerBound().x;
}

real BezierRev::xMax() const
{
	return bbox.getUpperBound().x;
}

real BezierRev::yMin() const
{
	return bbox.getLowerBound().y;
}

real BezierRev::yMax() const
{
	return bbox.getUpperBound().y;
}

real BezierRev::zMin() const
{
	return bbox.getLowerBound().z;
}

real BezierRev::zMax() const
{
	return bbox.getUpperBound().z;
}

Pos BezierRev::center() const
{
	return c;
}
