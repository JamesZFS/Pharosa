//
// Created by James on 2019/6/8.
//

#ifndef PHAROSA_VISIBLEPOINT_H
#define PHAROSA_VISIBLEPOINT_H

#include "../Pharosa.h"
#include "../core/Vec.h"

struct VisiblePoint
{
	Pos pos;    // position in the scene
	Dir wo;        // omega out
	Color Ld;    // direct radiance
	Color beta;    // throughput till vp
	Color tau;
	Color Phi;
	real N;        // total # photons received
	size_t M;        // current iteration # photons
	real r;        // interpolate radius

	VisiblePoint() : Ld{0, 0, 0}, beta{0, 0, 0}, tau{0, 0, 0}, Phi{0, 0, 0},
					 N(0), M(0), r(1.f)    // todo decide proper radius
	{
	}
};

typedef List<VisiblePoint *> VPPtrList;

#endif //PHAROSA_VISIBLEPOINT_H
