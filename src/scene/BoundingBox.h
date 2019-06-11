//
// Created by James on 2019/4/28.
//
//
// Created by James on 2019/4/19.
//

#ifndef PHAROSA_BOUNDINGBOX_H
#define PHAROSA_BOUNDINGBOX_H

#include "../Pharosa.h"
#include "Object.h"
#include "../alg/VisiblePoint.h"

// Bounding cube parallel to oxy, oyz, ozx
class BoundingBox
{
private:
	real xmin, xmax;
	real ymin, ymax;
	real zmin, zmax;

public:
	enum Axis
	{
		X = 0, Y = 1, Z = 2
	};

	BoundingBox();

	BoundingBox(const Pos &lower, const Pos &upper);

	BoundingBox(ObjectList::const_iterator begin, ObjectList::const_iterator end);

	BoundingBox(VPPtrList::const_iterator begin, VPPtrList::const_iterator end);

	inline Pos getLowerBound() const
	{ return {xmin, ymin, zmin}; }

	inline Pos getUpperBound() const
	{ return {xmax, ymax, zmax}; }

	void extendMargin(real margin);

	bool intersect(const Ray &ray) const;

	bool outsideSphere(const Pos &pos, real r) const;

	bool insideSphere(const Pos &pos, real r) const;

	Axis getLongestAxis() const;

	void report() const;
};

#endif //PHAROSA_BOUNDINGBOX_H
