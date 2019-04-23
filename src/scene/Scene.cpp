//
// Created by James on 2019/4/2.
//

#include "Scene.h"
#include "../utils/funcs.hpp"
//#include "../utils/Parser.h"


Scene::~Scene()
{
	for (Object *obj : singletons) {
		delete obj;
	}
	for (BoundingBox *box: bounding_boxes) {
		delete box;
	}
}

void Scene::fromObjFile(const String &obj_path)
{
	// todo implement this
}

void Scene::fromList(ObjectList &singletons_)
{
	singletons.insert(singletons.end(), singletons_.begin(), singletons_.end());
}

void Scene::append(Object *object)
{
	singletons.push_back(object);
}

//void Scene::appendMeshes(TriangleGroup meshes)
//{
//	singletons.insert(singletons.end(), meshes.begin(), meshes.end());
//}

bool Scene::intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const
{
	// naive for loop calculation
	// todo use OctTree, and define a new class named Object Combination (pure group, using enumeration)
	double ts = INF, tb = INF, t;	// intersection
	hit = nullptr;

	for (const auto *box:bounding_boxes) {
		if (box->intersectAny(ray, t, hit, x, normal) && t < tb) {
			tb = t;
		}
	}

	for (const Object *obj : singletons) {
		if (obj->geo->intersect(ray, t) && t < ts) {
			ts = t;
			if (t < tb) hit = obj;
		}
	}
	if (hit == nullptr) return false;    // no intersection

	if (ts < tb) { // hit singleton obj
		x = ray.org + ray.dir * ts;
		normal = hit->geo->normalAt(x);
	}
	// else: hit bounding box obj
	return true;
}

const Object * Scene::hitOf(const Ray &ray) const	// return hit without normal and x todo
{
	// todo Scene::hitOf() is not complete yet!
//	double t = INF, s;
	const Object *hit = nullptr;
	Pos x;
	Dir normal;
	intersectAny(ray, hit, x, normal);
	return hit;
//	for (const Object *obj : singletons) {
//		if (obj->geo->intersect(ray, s) && s < t) {
//			t = s;
//			hit = obj;
//		}
//	}
//	return hit;
}

size_t Scene::getSingletonCount()
{
	return singletons.size();
}

size_t Scene::getBoundingBoxCount()
{
	return bounding_boxes.size();
}

void Scene::fromList(BoundingBoxList &bounding_boxes_)
{
	bounding_boxes = std::move(bounding_boxes_);
}

void Scene::append(BoundingBox *box)
{
	bounding_boxes.push_back(box);
}
