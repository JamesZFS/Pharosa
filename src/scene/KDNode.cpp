//
// Created by James on 2019/4/28.
//

#include "KDNode.h"
#include "../geometric/Triangle.h"

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
bool KDNode::intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal, size_t depth) const
{
	// check bounding box first:
	if (tris == nullptr) {
		Buffer buffer;
		sprintf(buffer, "tris = null at depth = %ld\n", depth);
		throw String(buffer);
	}
	if (box == nullptr) {
		Buffer buffer;
		sprintf(buffer, "box =  null at depth = %ld\n", depth);
		throw String(buffer);
	}
	if (tris->empty() || !box->intersect(ray)) return false;

	// leaf case:
	if (l_child == nullptr || r_child == nullptr) {
		assert(l_child == nullptr && r_child == nullptr);
		bool intersected = false;
		double ti;
		// enumerate each tri:
		for (auto obj : *tris) {
			if (obj->geo->intersect(ray, ti)) {
				intersected = true;
				if (ti < t) {    // *** update first intersection
					t = ti;
					hit = obj;
					x = ray.org + ray.dir * t;
					normal = obj->geo->normalAt(x);
				}
			}
		}
		return intersected;
	}

	// normal case, recurse:
	bool l_hit = l_child->intersectAny(ray, t, hit, x, normal, depth + 1);
	bool r_hit = r_child->intersectAny(ray, t, hit, x, normal, depth + 1);
	return l_hit || r_hit;
}

#define MATCH_THRESHOLD 0.8

// !
void KDNode::build(const ObjectList &triangles, size_t depth)
{
	__max_depth__ = max2(__max_depth__, depth);
	box = new BoundingBox(triangles);    // bound all triangles
	tris = new ObjectList(triangles);    // copy triangle ptrs

	// base case:
	if (tris->size() <= 1) return;

	// get center point of all tris:
	Pos center;
	for (auto obj: *tris) {
		auto tr = dynamic_cast<Triangle *>(obj->geo);
		assert(tr != nullptr);
		center += tr->center();
	}
	center /= tris->size();

	// sort tris to left or right range:
	auto l_tris = new ObjectList, r_tris = new ObjectList;
	auto cur_axis = box->getLongestAxis();
	for (auto obj: *tris) {
		auto tr = dynamic_cast<Triangle *>(obj->geo);
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
