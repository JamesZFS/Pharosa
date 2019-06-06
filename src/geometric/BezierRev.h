//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_BEZIERREV_H
#define PHAROSA_BEZIERREV_H

#include "PolyRev.h"
#include "Finite.h"

// revolution solid of polynomial, assuming the revolution is along X-axis
struct BezierRev : PolyRev
{
	List<Arr<real, 2>> ctrl_pts;

	BezierRev(List<Arr<real, 2>> &&control_pts_);

	Type type() const override
	{ return BEZIERREV; }

	// todo
//	real xMin() const override;
//
//	real xMax() const override;
//
//	real yMin() const override;
//
//	real yMax() const override;
//
//	real zMin() const override;
//
//	real zMax() const override;
//
//	Pos center() const override;

	static BezierRev *acquire(const Json &json);
};


#endif //PHAROSA_BEZIERREV_H
