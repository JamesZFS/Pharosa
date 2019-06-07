//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_RAY_HPP
#define PHAROSA_RAY_HPP

#include "Vec.h"

// L1 definition
struct Ray
{
	Pos org;
	Dir dir;

	Ray() = default;

	Ray(const Pos &org_, const Dir &dir_) : org(org_), dir(dir_)
	{}

	inline Pos posAfter(real t) const
	{ return org + dir * t; }

	inline Ray &offset(real t)
	{
		org += dir * t;
		return *this;
	}
};

#endif //PHAROSA_RAY_HPP
