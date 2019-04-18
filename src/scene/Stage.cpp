//
// Created by James on 2019/4/2.
//

#include "Stage.h"
//#include "../utils/json.hpp"
#include "../utils/funcs.hpp"
//#include "../utils/Parser.h"


Stage::~Stage()
{
	for (Object *obj : objects) {
		delete obj;
	}
}

void Stage::fromJsonFile(const String &config_path)
{
	// todo
//	objects = std::move(Parser::fromJsonFile(config_path));
}

void Stage::fromObjFile(const String &obj_path)
{
	// todo implement this
}

void Stage::fromObjectList(ObjectList &&objects_)
{
	objects = std::move(objects_);
}

void Stage::append(Object *object)
{
	objects.push_back(object);
}

//void Stage::appendMeshes(TriangleGroup meshes)
//{
//	objects.insert(objects.end(), meshes.begin(), meshes.end());
//}

bool Stage::intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const
{
	// naive for loop calculation
	// todo use OctTree
	double t = INF, s;	// intersection
	hit = nullptr;
	for (const Object *obj : objects) {
		if (obj->geo->intersect(ray, s) && s < t) {
			t = s;
			hit = obj;
		}
	}
	if (hit == nullptr) return false;    // no intersection
	x = ray.org + ray.dir * t;
	normal = hit->geo->normalAt(x);
	return true;
}

const Object * Stage::hitOf(const Ray &ray) const	// return hit without normal and x
{
	// naive for loop calculation
	// todo use OctTree
	double t = INF, s;
	const Object *hit = nullptr;
	for (const Object *obj : objects) {
		if (obj->geo->intersect(ray, s) && s < t) {
			t = s;
			hit = obj;
		}
	}
	return hit;
}

unsigned int Stage::getObjectCount()
{
	return (unsigned int) objects.size();
}
