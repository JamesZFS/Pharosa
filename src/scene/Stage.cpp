//
// Created by James on 2019/4/2.
//

#include "Stage.h"
//#include "../utils/json.hpp"
#include "../utils/funcs.hpp"
//#include "../utils/Parser.h"


Stage::~Stage()
{
	for (Object *obj : singletons) {
		delete obj;
	}
	for (BoundingBox *box: bounding_boxes) {
		delete box;
	}
}

void Stage::fromJsonFile(const String &config_path)
{
	// todo
//	singletons = std::move(Parser::fromJsonFile(config_path));
}

void Stage::fromObjFile(const String &obj_path)
{
	// todo implement this
}

void Stage::fromList(ObjectList &singletons_)
{
	singletons = std::move(singletons_);
}

void Stage::append(Object *object)
{
	singletons.push_back(object);
}

//void Stage::appendMeshes(TriangleGroup meshes)
//{
//	singletons.insert(singletons.end(), meshes.begin(), meshes.end());
//}

bool Stage::intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const
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

const Object * Stage::hitOf(const Ray &ray) const	// return hit without normal and x todo
{
	// todo Stage::hitOf() is not complete yet!
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

size_t Stage::getSingletonCount()
{
	return singletons.size();
}

size_t Stage::getBoundingBoxCount()
{
	return bounding_boxes.size();
}

void Stage::fromList(BoundingBoxList &bounding_boxes_)
{
	bounding_boxes = std::move(bounding_boxes_);
}

void Stage::append(BoundingBox *box)
{
	bounding_boxes.push_back(box);
}
