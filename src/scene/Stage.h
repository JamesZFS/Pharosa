//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_STAGE_H
#define RAYTRACKER_STAGE_H

#include "../lib.h"
#include "../Ray.hpp"
#include "All.h"

// Stage class, for i/o models and calculating ray intersection
class Stage
{
public:
	List<Scenes::Object *> objs;
public:
	Stage() = default;        // init an empty stage

	void from_obj(const String &obj_path);        // load triangles from obj file

	// calculate first intersection with all objects
	bool intersectAny(const Ray &ray, double &t, const Scenes::Object *touched) const;

};

#include "Stage.cpp"

#endif //RAYTRACKER_STAGE_H
