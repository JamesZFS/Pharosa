//
// Created by think on 2019/4/2.
//

#ifndef RAYTRACKER_RAY_HPP
#define RAYTRACKER_RAY_HPP

#include "Vec.hpp"

// ray definition
struct Ray
{
	Pos org;
	Dir dir;

	Ray() = default;

	Ray(const Pos &org_, const Dir &dir_) : org(org_), dir(dir_)
	{}
};

#endif //RAYTRACKER_RAY_HPP
