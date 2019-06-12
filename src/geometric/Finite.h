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
	virtual real xMin() const = 0;// left most

	virtual real xMax() const = 0;// right most

	virtual real yMin() const = 0;

	virtual real yMax() const = 0;

	virtual real zMin() const = 0;

	virtual real zMax() const = 0;

	virtual Pos center() const = 0;// get center point

	virtual real area() const = 0;// surface area
};

#endif //PHAROSA_FINITE_H
