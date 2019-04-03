//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_STAGE_H
#define RAYTRACKER_STAGE_H

#include "../lib.h"
#include "../Ray.hpp"
#include "All.h"

typedef List<const Scenes::Object *> ObjectGroup;

// Stage class, for i/o models and calculating ray intersection
class Stage
{
private:
	ObjectGroup objects;	// using pointer to allow polymorphism

public:
	Stage() = default;        // init an empty stage

	~Stage();

	void fromJsonFile(const String &config_path);	// load objects from json file

	void fromObjFile(const String &obj_path);      // load triangles from obj file

	void fromList(ObjectGroup &objects_);	// load from list

	// calculate first intersection with all objects
	bool intersectAny(const Ray &ray, double &t, const Scenes::Object *&hit) const;

};

#include "Stage.cpp"

#endif //RAYTRACKER_STAGE_H
