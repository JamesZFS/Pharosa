//
// Created by think on 2019/4/2.
//

#ifndef PHAROSA_RAY_HPP
#define PHAROSA_RAY_HPP

#include "Vec.h"

// ray definition
struct Ray
{
	Pos org;
	Dir dir;

	Ray() = default;

	Ray(const Pos &org_, const Dir &dir_) : org(org_), dir(dir_)
	{}

	inline static Ray between(const Pos &from, const Pos &to)
	{ return {from, to - from}; }
};

#endif //PHAROSA_RAY_HPP
