//
// Created by think on 2019/4/2.
//

#ifndef JRT_RAY_HPP
#define JRT_RAY_HPP

#include "Vec.hpp"

// ray definition
struct Ray
{
	Pos org;
	Dir dir;

	Ray(Pos org_, Dir dir_) : org(org_), dir(dir_)
	{}
};

#endif //JRT_RAY_HPP
