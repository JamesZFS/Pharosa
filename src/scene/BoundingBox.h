//
// Created by James on 2019/4/28.
//
//
// Created by James on 2019/4/19.
//

#ifndef PHAROSA_BOUNDINGBOX_H
#define PHAROSA_BOUNDINGBOX_H

#include "../defs.h"
#include "Object.h"

// Bounding cube parallel to oxy, oyz, ozx
class BoundingBox
{
private:
	double xmin, xmax;
	double ymin, ymax;
	double zmin, zmax;

public:
	enum Axis {X = 0, Y = 1, Z = 2};

	BoundingBox();

	BoundingBox(const ObjectList &triangles);

	bool intersect(const Ray &ray) const;

	Axis getLongestAxis() const;

	void report() const;
};

#endif //PHAROSA_BOUNDINGBOX_H
