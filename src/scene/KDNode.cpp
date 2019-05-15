//
// Created by James on 2019/4/28.
//

#include "KDNode.h"
#include "../geometric/Finite.h"
#include "Intersection.hpp"

KDNode::KDNode() : box(nullptr), tris(nullptr), l_child(nullptr), r_child(nullptr)
{

}

// !
KDNode::KDNode(const ObjectList &triangles) : KDNode()
{
	build(triangles, 0);    // build from root
	box->report();
}

KDNode::~KDNode()
{
	delete box;
	delete tris;
	delete l_child;
	delete r_child;
}

// !!
void KDNode::intersect(const Ray &ray, double &t, Intersection &isect) const
{
	// check bounding box first:
	if (tris->empty() || !box->intersect(ray)) return;    // degenerate case

	// leaf case:
	if (l_child == nullptr || r_child == nullptr) {
		// enumerate each tri:
		for (const Object *obj : *tris) {
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

#define MATCH_THRESHOLD 0.5

// !
void KDNode::build(const ObjectList &triangles, size_t depth)
{
	__max_depth__ = max2(__max_depth__, depth);
	box = new BoundingBox(triangles);    // bound all triangles
	tris = new ObjectList(triangles);    // copy Finite ptrs

	// base case:
	if (tris->size() <= 1) return;

	// get center point of all tris:
	Pos center;
	for (auto obj: *tris) {
		auto tr = dynamic_cast<Finite *>(obj->geo);
		assert(tr != nullptr);
		center += tr->center();
	}
	center /= tris->size();

	// sort tris to left or right range:
	auto l_tris = new ObjectList, r_tris = new ObjectList;
	auto cur_axis = box->getLongestAxis();
	for (auto obj: *tris) {
		auto tr = dynamic_cast<Finite *>(obj->geo);
		switch (cur_axis) {
			case BoundingBox::X:
				tr->center().x <= center.x ? l_tris->push_back(obj) : r_tris->push_back(obj);
				break;
			case BoundingBox::Y:
				tr->center().y <= center.y ? l_tris->push_back(obj) : r_tris->push_back(obj);
				break;
			case BoundingBox::Z:
				tr->center().z <= center.z ? l_tris->push_back(obj) : r_tris->push_back(obj);
				break;
		}
	}

	// ???
//	if (l_tris->empty() && !r_tris->empty()) *l_tris = *r_tris;
//	if (r_tris->empty() && !l_tris->empty()) *r_tris = *l_tris;

	// stop subdividing condition:
	float n_match = 0;
	for (auto lo : *l_tris) {
		for (auto ro : *r_tris) {
			if (lo == ro) {
				++n_match;    // found match
				++__counter__;
				break;
			}
		}
	}

	if (n_match / l_tris->size() < MATCH_THRESHOLD && n_match / r_tris->size() < MATCH_THRESHOLD) {
		l_child = new KDNode;
		l_child->build(*l_tris, depth + 1);
		r_child = new KDNode;
		r_child->build(*r_tris, depth + 1);
	}
	// else: stop dividing
}

#undef MATCH_THRESHOLD
