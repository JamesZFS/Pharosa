//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_STAGE_H
#define PHAROSA_STAGE_H

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

	/** judge and calculate first intersection with all objects
	 * @param ray
	 * @param hit : pointer of object first hit
	 * @param x : hitting point
	 * @param normal : normal vector of hitting point
	 * @return true if intersects any object else false
	 */
	bool intersectAny(const Ray &ray, const Scenes::Object *&hit, Pos &x, Dir &normal) const;

};

#include "Stage.cpp"

#endif //PHAROSA_STAGE_H
