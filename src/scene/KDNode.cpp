//
// Created by James on 2019/4/28.
//

#include "KDNode.h"
#include "../geometric/Finite.h"
#include "Intersection.h"

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
	if (objs->empty() || !box->intersect(ray)) return;    // degenerate case

	// leaf case: todo possible accelerate by eliminating obj list
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
	box = new BoundingBox(finite_objs.cbegin(), finite_objs.cend());    // bound all finite_objs
	objs = new ObjectList(finite_objs);    // copy Finite ptrs

	// base case:
	if (objs->size() <= 1) return;

	// get center point of all objs:
	Pos center;
	for (auto obj: *objs) {
		auto shape = dynamic_cast<Finite *>(obj->geo);
		assert(shape != nullptr);
		center += shape->center();
	}
	center /= objs->size();

	// sort objs to left or right range:
	auto l_objs = new ObjectList, r_objs = new ObjectList;
	auto cur_axis = box->getLongestAxis();
	for (auto obj: *objs) {
		auto shape = dynamic_cast<Finite *>(obj->geo);
		switch (cur_axis) {
			case BoundingBox::X:
				shape->center().x <= center.x ? l_objs->push_back(obj) : r_objs->push_back(obj);
				break;
			case BoundingBox::Y:
				shape->center().y <= center.y ? l_objs->push_back(obj) : r_objs->push_back(obj);
				break;
			case BoundingBox::Z:
				shape->center().z <= center.z ? l_objs->push_back(obj) : r_objs->push_back(obj);
				break;
		}
	}

	// recursively build
	l_child = new KDNode;
	l_child->build(*l_objs, depth + 1);
	r_child = new KDNode;
	r_child->build(*r_objs, depth + 1);
}
