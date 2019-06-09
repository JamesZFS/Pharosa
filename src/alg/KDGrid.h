//
// Created by James on 2019/6/8.
//

#ifndef PHAROSA_KDGRID_H
#define PHAROSA_KDGRID_H

#include "VisiblePoint.h"
#include "../scene/Intersection.h"
#include "../scene/BoundingBox.h"

// a KD tree that contains visible points (as weak refrence)
class KDGrid
{
private:
	BoundingBox *box;
	VPPtrList *vps;		// weak ptrs
	KDGrid *l_child;
	KDGrid *r_child;

	// build from [beign, end) in vplist
	void build(VPPtrList::const_iterator begin, VPPtrList::const_iterator end, size_t depth);

public:
	KDGrid() : box(nullptr), vps(nullptr), l_child(nullptr), r_child(nullptr)
	{}

	explicit KDGrid(const VPPtrList &vplist);

	~KDGrid();

	// find all vps in the vicinity of pos whose r is below r_bound
	bool query(const Pos &pos, real r_bound, VPPtrList &vps_out, size_t depth = 0);
};


class NaiveGrid
{
private:
	VPPtrList vps;
public:
	explicit NaiveGrid(const VPPtrList &vplist) : vps(vplist) {}

	bool query(const Pos &pos, real r_bound, VPPtrList &vps_out);
};


#endif //PHAROSA_KDGRID_H
