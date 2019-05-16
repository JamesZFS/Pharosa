//
// Created by James on 2019/5/16.
//

#include "BezierRev.h"
#include "../utils/funcs.hpp"

BezierRev::BezierRev(List<Arr<double, 2>> &&control_pts_) : PolyRev(), ctrl_pts(std::move(control_pts_))
{
	auto n = (unsigned char) (ctrl_pts.size() - 1);
	for (unsigned char i = 0; i <= n; ++i) {
		auto Bi = Polynomial::binomial(0, 1, i) * Polynomial::binomial(1, -1, n - i) * Funcs::binomial(n, i);
		phi += Bi * ctrl_pts[i][0];
		psi += Bi * ctrl_pts[i][1];
	}
	psi_2 = psi * psi;
	debug("Received Bezier Curve Eqa:\n");
	phi.report();
	psi.report();
	debug("\n");
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
