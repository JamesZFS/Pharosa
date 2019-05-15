//
// Created by James on 2019/5/16.
//

#ifndef PHAROSA_FINITE_H
#define PHAROSA_FINITE_H

#include "../Pharosa.h"
#include "../core/Vec.h"

// size finite class, interface
struct Finite
{
	virtual double xMin() const = 0;// left most

	virtual double xMax() const = 0;// right most

	virtual double yMin() const = 0;

	virtual double yMax() const = 0;

	virtual double zMin() const = 0;

	virtual double zMax() const = 0;

	virtual Pos center() const = 0;// get center point
};

#endif //PHAROSA_FINITE_H
