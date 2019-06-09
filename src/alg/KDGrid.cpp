//
// Created by James on 2019/6/8.
//

#include "KDGrid.h"

#include <algorithm>


KDGrid::KDGrid(const VPPtrList &vplist) : KDGrid()
{
	build(vplist.cbegin(), vplist.cend(), 0);    // build from root
	box->report();
}

KDGrid::~KDGrid()
{
	delete box;
	delete vps;
	delete l_child;
	delete r_child;
}

bool VPCompX(VisiblePoint *a, const VisiblePoint *b)
{
	return a->pos.x < b->pos.x;
}

bool VPCompY(VisiblePoint *a, const VisiblePoint *b)
{
	return a->pos.y < b->pos.y;
}

bool VPCompZ(VisiblePoint *a, const VisiblePoint *b)
{
	return a->pos.z < b->pos.z;
}

void KDGrid::build(VPPtrList::const_iterator begin, VPPtrList::const_iterator end, size_t depth)
{
	__kdgrid_max_depth__ = max2(__kdgrid_max_depth__, depth);
	box = new BoundingBox(begin, end);    // bound all vps
	vps = new VPPtrList(begin, end);    // copy Finite ptrs

	// base case:
	if (vps->size() <= 10) return;

	// sort vps to left or right range:
	auto cur_axis = box->getLongestAxis();
	auto mid = vps->size() / 2;
	switch (cur_axis) {
		case BoundingBox::X:
			std::nth_element(vps->begin(), vps->begin() + mid, vps->end(), VPCompX);
			break;
		case BoundingBox::Y:
			std::nth_element(vps->begin(), vps->begin() + mid, vps->end(), VPCompY);
			break;
		case BoundingBox::Z:
			std::nth_element(vps->begin(), vps->begin() + mid, vps->end(), VPCompZ);
			break;
	}

	// recurse
	l_child = new KDGrid;
	l_child->build(begin, begin + mid, depth + 1);
	r_child = new KDGrid;
	r_child->build(begin + mid, end, depth + 1);
}

bool KDGrid::query(const Pos &pos, real r_bound, VPPtrList &vps_out, size_t depth)
{
	__kdgrid_max_depth__ = max2(__kdgrid_max_depth__, depth);
	if (box->outsideSphere(pos, r_bound)) {  // base case 1
		return false;
	}
	if (l_child == nullptr || box->insideSphere(pos, r_bound)) {	// base case 2 or leaf
		// try to report all
		assert((l_child == nullptr) == (r_child == nullptr));
		bool found;
		std::for_each(vps->begin(), vps->end(), [&](VisiblePoint *vp) {
			if (!vp->beta.isBlack() && (vp->pos - pos).sqr() <= vp->r * vp->r) {    // filter unwanted
				found = true;
				vps_out.push_back(vp);
			}
		});
		return found;
	}
	bool fl = l_child->query(pos, r_bound, vps_out, depth + 1);
	bool fr = r_child->query(pos, r_bound, vps_out, depth + 1);
	return fl || fr;
}

bool NaiveGrid::query(const Pos &pos, real, VPPtrList &vps_out)
{
	bool found = false;
	for (auto vp : vps) {
		if (!vp->beta.isBlack() && (vp->pos - pos).norm() <= vp->r) {
			found = true;
			vps_out.push_back(vp);
		}
	}
	return found;
}
