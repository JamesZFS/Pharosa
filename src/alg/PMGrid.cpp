//
// Created by James on 2019/6/8.
//

#include "PMGrid.h"

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

void KDGrid::query(const Pos &pos, real r_bound, const QueryCallback &callback)
{
	if (box->outsideSphere(pos, r_bound))  // base case 1
		return;
	if (l_child == nullptr || box->insideSphere(pos, r_bound)) {    // base case 2 or leaf
		// try to report all
		assert((l_child == nullptr) == (r_child == nullptr));
		for (auto vp : *vps) {
			if (!vp->beta.isBlack() && (vp->pos - pos).sqr() <= vp->r * vp->r) {    // filter unwanted
				callback(vp);
			}
		}
//		if (found)
//			safe_debug("count = %ld\n", cnt);
		return;
	}
	l_child->query(pos, r_bound, callback);
	r_child->query(pos, r_bound, callback);
}

void NaiveGrid::query(const Pos &pos, real, const QueryCallback &callback)
{
	for (auto vp : vps) {
		if (!vp->beta.isBlack() && (vp->pos - pos).sqr() <= vp->r * vp->r) {
			callback(vp);
		}
	}
}

// == uniform grid ==

const int UniformGrid::n_grid = 220;    // todo param

UniformGrid::UniformGrid(const VPPtrList &vplist) :
		grids(size_t(n_grid), List2D<VPPtrList>(size_t(n_grid), List<VPPtrList>(size_t(n_grid))))
{
	BoundingBox bbox(vplist.begin(), vplist.end());
	real r_bound = 0;
	for (auto vp : vplist) {
		r_bound = max2(r_bound, vp->r);
	}
	bbox.extendMargin(r_bound);
	bbox.report();
	fflush(stdout);
	lower = bbox.getLowerBound();
	upper = bbox.getUpperBound();
	d = (upper - lower) / n_grid;
	debug("d: ");
	d.report(true);
	for (auto vp : vplist) {
		storeToGrid(vp);
	}
}

void UniformGrid::storeToGrid(VisiblePoint *vp)
{
	auto lo = mapToGrid({vp->pos.x - vp->r, vp->pos.y - vp->r, vp->pos.z - vp->r});
	auto hi = mapToGrid({vp->pos.x + vp->r, vp->pos.y + vp->r, vp->pos.z + vp->r});
	auto i_lower = max2(lo.x, 0), i_upper = min2(hi.x + 1, n_grid);
	auto j_lower = max2(lo.y, 0), j_upper = min2(hi.y + 1, n_grid);
	auto k_lower = max2(lo.z, 0), k_upper = min2(hi.z + 1, n_grid);
	for (auto i = i_lower; i < i_upper; ++i) {
		for (auto j = j_lower; j < j_upper; ++j) {
			for (auto k = k_lower; k < k_upper; ++k) {
				BoundingBox box(mapToScene(i, j, k), mapToScene(i + 1, j + 1, k + 1));
				if (box.outsideSphere(vp->pos, vp->r))
					continue;
				grids[i][j][k].push_back(vp);
			}
		}
	}
}

void UniformGrid::query(const Pos &pos, real r_bound, const QueryCallback &callback)
{
	NOT_USED(r_bound);
	auto index = mapToGrid(pos);
	if (index.x < 0 || n_grid <= index.x || index.y < 0 || n_grid <= index.y || index.z < 0 || n_grid <= index.z)
		return;
	auto &vps = grids[index.x][index.y][index.z];
	for (auto vp : vps) {
		if (!vp->beta.isBlack() && (vp->pos - pos).sqr() <= vp->r * vp->r) {
			callback(vp);
		}
	}
}
