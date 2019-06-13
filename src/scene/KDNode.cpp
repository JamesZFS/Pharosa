//
// Created by James on 2019/4/28.
//

#include "KDNode.h"
#include "../geometric/Finite.h"
#include "Intersection.h"

#include <algorithm>

KDNode::KDNode() : box(nullptr), objs(nullptr), l_child(nullptr), r_child(nullptr)
{

}

// !
KDNode::KDNode(const ObjectList &finite_objs) : KDNode()
{
	build(finite_objs, 0);    // build from root
	box->report();
}

KDNode::~KDNode()
{
	delete box;
	delete objs;
	delete l_child;    // delete recursively
	delete r_child;
}

// !!
void KDNode::intersect(const Ray &ray, real &t, Intersection &isect) const
{
	// check bounding box first:
	if (!box->intersect(ray)) return;    // degenerate case

	// leaf case:
	if (l_child == nullptr || r_child == nullptr) {
		// enumerate each tri:
		for (const Object *obj : *objs) {
			if (obj->geo->intersect(ray, t, isect)) {    // *** update first intersection
				isect.hit = obj;
			}
		}
		return;
	}

	// common case, recurse:
	l_child->intersect(ray, t, isect);
	r_child->intersect(ray, t, isect);
}

// !
void KDNode::build(const ObjectList &finite_objs, size_t depth)
{
	__kdnode_max_depth__ = max2(__kdnode_max_depth__, depth);
	objs = new ObjectList;
	box = new BoundingBox(finite_objs.cbegin(), finite_objs.cend());    // bound_pts all finite_objs

	// base case:
	if (finite_objs.size() <= 5) {
		objs->assign(finite_objs.cbegin(), finite_objs.cend());    // copy Finite ptrs
		return;
	}

	// get center point of all objs:
	Pos center;
	for (auto obj: finite_objs) {
		auto shape = dynamic_cast<Finite *>(obj->geo);
		assert(shape != nullptr);
		center += shape->center();
	}
	center /= finite_objs.size();

	// sort objs to left or right range:
	ObjectList l_objs, r_objs;
	auto cur_axis = box->getLongestAxis();
	for (auto obj: finite_objs) {
		auto shape = dynamic_cast<Finite *>(obj->geo);
		switch (cur_axis) {
			case BoundingBox::X:
				shape->center().x <= center.x ? l_objs.push_back(obj) : r_objs.push_back(obj);
				break;
			case BoundingBox::Y:
				shape->center().y <= center.y ? l_objs.push_back(obj) : r_objs.push_back(obj);
				break;
			case BoundingBox::Z:
				shape->center().z <= center.z ? l_objs.push_back(obj) : r_objs.push_back(obj);
				break;
		}
	}
	// stop subdividing condition:
	auto n_match = std::count_if(l_objs.cbegin(), l_objs.cend(), [&r_objs](Object *obj) -> bool {
		return std::find(r_objs.cbegin(), r_objs.cend(), obj) != r_objs.cend();
	});

	if (n_match < 0.5 * l_objs.size() && n_match < 0.5 * r_objs.size()) {
		l_child = new KDNode;
		l_child->build(l_objs, depth + 1);    // recursively build
		r_child = new KDNode;
		r_child->build(r_objs, depth + 1);
	}
	else { // stop dividing
		objs->assign(finite_objs.cbegin(), finite_objs.cend());
	}
}
