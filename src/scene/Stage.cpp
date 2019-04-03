//
// Created by James on 2019/4/2.
//

#include "Stage.h"
//#include "../utils/json.hpp"
#include "../utils/funcs.hpp"
#include "../utils/Parser.h"


void Stage::from_json(const String &config_path)
{
	objects = std::move(Parser::fromJsonFile(config_path));
}

void Stage::from_obj(const String &obj_path)
{
	// todo implement this
}

bool Stage::intersectAny(const Ray &ray, double &t, const Scenes::Object *hit) const
{
	// naive for loop calculation
	// todo use OctTree
	t = INF;
	hit = nullptr;
	for (const Scenes::Object *obj : objects) {
		double s = obj->intersect(ray);
		if (s > EPS && s < t) {	// if s < 0, meaning no hitting, continue
			t = s;
			hit = obj;
		}
	}
	return (hit != nullptr);    // if no intersection, return false
}

Stage::~Stage()
{
	for (const Scenes::Object *obj : objects) {
		delete obj;
	}
}
