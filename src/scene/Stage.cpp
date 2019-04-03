//
// Created by James on 2019/4/2.
//

#include "Stage.h"
//#include "../utils/json.hpp"
#include <nlohmann/json.hpp>

void Stage::from_config(const String &config_path)
{
	std::ifstream fin;
	fin.open(config_path, std::ios::in);
	using json = nlohmann::json;

	char buffer[250];
	if (fin.is_open()) {
		fin.getline(buffer, 200);
		// todo
	}
	else {
		sprintf(buffer, "IO Error: config_path \"%s\" cannot be opened, reading stopped.", config_path.data());
		warn(buffer);
		exit(1);
	}
	fin.close();
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
