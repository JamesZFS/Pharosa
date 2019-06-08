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
public:
	BoundingBox *box;
	VPPtrList *vps;		// weak ptrs
	KDGrid *l_child;
	KDGrid *r_child;

	// build from [beign, end) in vplist
	void build(VPPtrList::iterator begin, VPPtrList::iterator end, size_t depth);

public:
	KDGrid() : box(nullptr), vps(nullptr), l_child(nullptr), r_child(nullptr)
	{}

	explicit KDGrid(VPPtrList vplist);

	~KDGrid();

	// find all vps in the vicinity of pos whose r is below r_bound
	bool query(const Pos &pos, real r_bound, VPPtrList &vps_out);
};


#endif //PHAROSA_KDGRID_H
