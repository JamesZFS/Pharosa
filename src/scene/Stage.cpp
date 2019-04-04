//
// Created by James on 2019/4/2.
//

#include "Stage.h"
//#include "../utils/json.hpp"
#include "../utils/funcs.hpp"
//#include "../utils/Parser.h"


Stage::~Stage()
{
	for (const Scenes::Object *obj : objects) {
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

void Stage::fromList(ObjectGroup &objects_)
{
	objects = std::move(objects_);
}

bool Stage::intersectAny(const Ray &ray, const Scenes::Object *&hit, Pos &x, Dir &normal) const
{
	// naive for loop calculation
	// todo use OctTree
	double t = INF;	// intersection
	hit = nullptr;
	for (const Scenes::Object *obj : objects) {
		double s = obj->intersect(ray, t);
		if (s > EPS && s < t) {	// if s < 0, i.e. no hitting, continue
			t = s;
			hit = obj;
		}
	}
	if (hit == nullptr) return false;    // no intersection
	x = ray.org + ray.dir * t;
	normal = hit->normalAt(x);
	return true;
}
