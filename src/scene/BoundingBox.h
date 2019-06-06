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

// Bounding cube parallel to oxy, oyz, ozx
class BoundingBox
{
private:
	real xmin, xmax;
	real ymin, ymax;
	real zmin, zmax;

public:
	enum Axis {X = 0, Y = 1, Z = 2};

	BoundingBox();

	BoundingBox(const ObjectList &triangles);

	bool intersect(const Ray &ray) const;

	Axis getLongestAxis() const;

	void report() const;
};

#endif //PHAROSA_BOUNDINGBOX_H
