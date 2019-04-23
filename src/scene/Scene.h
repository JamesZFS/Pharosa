//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_SCENE_H
#define PHAROSA_SCENE_H

#include "../lib.h"
#include "../core/Ray.hpp"
#include "Object.h"
#include "bounding/All.h"

// Scene class, for i/o models and calculating ray intersection
class Scene
{
#ifdef __DEV_STAGE__
public:
#else
private:
#endif
	ObjectList singletons;	// all singleton object ptrs, using pointer to allow polymorphism
	BoundingBoxList bounding_boxes;	// all bounding boxes ptrs

public:
	Scene() = default;        // init an empty scene

	Scene(const Json &json);	// init from json

	~Scene();

	size_t getSingletonCount();	// count singletons in total

	size_t getBoundingBoxCount();	// count BoundingBox in total
	
	void fromObjFile(const String &obj_path);      // load triangles from obj file

	void fromList(ObjectList &singletons_);	// load from object ptr list (! move constructor)

	void fromList(BoundingBoxList &bounding_boxes_); 	// from bounding box ptr list

	void append(Object *object);	// append an object (affiliates it)

	void append(BoundingBox *box);	// append a bounding box (affiliates it)

//	void appendMeshes(TriangleGroup meshes);  // append mesh segments (affiliates it)todo
	// meshes should be excluded from ObjectList and installed via appendMeshes method

	/** judge and calculate first intersection with all objects
	 * @param ray
	 * @param hit : pointer of object first hit
	 * @param x : hitting point
	 * @param normal : normal vector of hitting point
	 * @return true if intersects any object else false
	 */
	bool intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const;

	const Object * hitOf(const Ray &ray) const;	// compute only the first hit target of ray
};

#endif //PHAROSA_SCENE_H
