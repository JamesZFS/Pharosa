//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_BEZIERREV_H
#define PHAROSA_BEZIERREV_H

#include "PolyRev.h"
#include "Finite.h"

// revolution solid of polynomial, assuming the revolution is along X-axis
struct BezierRev : PolyRev, Finite
{
	List<Arr<double, 2>> ctrl_pts;

	BezierRev(List<Arr<double, 2>> &&control_pts_);

	// todo
	double xMin() const override;

	double xMax() const override;

	double yMin() const override;

	double yMax() const override;

	double zMin() const override;

	double zMax() const override;

	Pos center() const override;

	static BezierRev *acquire(const Json &json);
};


#endif //PHAROSA_BEZIERREV_H
