//
// Created by James on 2019/4/2.
//

#include "Stage.h"

Stage::Stage(const List<Object> &objects_)
{
	objs = objects_;
}

void Stage::from_obj(const String &obj_path)
{
	// todo implement this
}

bool Stage::intersectAny(const Ray &ray, double &t, const Object *touched) const
{
	// naive for loop calculation
	// todo use OctTree
	t = INF;
	touched = nullptr;
	for (const Object &obj : objs) {
		double s = obj.intersect(ray);
		if (s > EPS && s < t) {	// if s < 0, meaning no hitting, continue
			t = s;
			touched = &obj;
		}
	}
	return (touched != nullptr);    // if no intersection, return false
}
