//
// Created by James on 2019/5/16.
//

#include "BezierRev.h"
#include "../utils/funcs.hpp"

BezierRev::BezierRev(List<Arr<double, 2>> &&control_pts_) : PolyRev(), ctrl_pts(std::move(control_pts_))
{
	phi.report();
	psi.report();
	assert(2 <= ctrl_pts.size() && ctrl_pts.size() <= 10);
	for (auto &p : ctrl_pts) {
		debug("[ %.2f, %.2f ]  ", p[0], p[1]);
	}
	debug("\n");
	auto n = (unsigned char) (ctrl_pts.size() - 1);
	for (unsigned char i = 0; i <= n; ++i) {
		auto Bi = Polynomial::binomial(0, 1, i) * Polynomial::binomial(1, -1, n - i) * Funcs::binomial(n, i);
		debug("Bi\n");
		Bi.report();
		debug("\n");
		debug("phi\n");
		phi += Bi * ctrl_pts[i][0];
		debug("psi\n");
		psi += Bi * ctrl_pts[i][1];
	}
	psi_2 = psi * psi;
	phi.report();
	psi.report();
}

double BezierRev::xMin() const
{
	return 0;
}

double BezierRev::xMax() const
{
	return 0;
}

double BezierRev::yMin() const
{
	return 0;
}

double BezierRev::yMax() const
{
	return 0;
}

double BezierRev::zMin() const
{
	return 0;
}

double BezierRev::zMax() const
{
	return 0;
}

Pos BezierRev::center() const
{
	return Pos();
}
