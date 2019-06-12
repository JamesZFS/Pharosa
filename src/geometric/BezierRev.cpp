//
// Created by James on 2019/5/16.
//

#include "BezierRev.h"
#include "../utils/funcs.hpp"

BezierRev::BezierRev(List<Arr<real, 2>> &&control_pts_) : PolyRev(), ctrl_pts(std::move(control_pts_))
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

void BezierRev::report() const
{
	printf("<Bezier Revolution> control points: ");
	for (auto cp : ctrl_pts) {
		printf("(%f, %f) ", cp[0], cp[1]);
	}
	printf("\n");
	PolyRev::report();
}
