//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_BEZIERREV_H
#define PHAROSA_BEZIERREV_H

#include "PolyRev.h"
#include "Finite.h"
#include "../scene/BoundingBox.h"

// revolution solid of polynomial, assuming the revolution is along X-axis
struct BezierRev : PolyRev, Finite
{
	List<Pos> bound_pts;
	Pos c;
	BoundingBox bbox;

	BezierRev(const List<Arr<real, 2>> &ctrl_pts, real scale = 1);

	Type type() const override
	{ return BEZIERREV; }

	void applyTransform(const TransMat &mat_) override;

	void report() const override;

	real xMin() const override;

	real xMax() const override;

	real yMin() const override;

	real yMax() const override;

	real zMin() const override;

	real zMax() const override;

	Pos center() const override;

	static BezierRev *acquire(const Json &json);
};


#endif //PHAROSA_BEZIERREV_H
