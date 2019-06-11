//
// Created by James on 2019/6/8.
//

#ifndef PHAROSA_KDGRID_H
#define PHAROSA_KDGRID_H

#include "VisiblePoint.h"
#include "../scene/Intersection.h"
#include "../scene/BoundingBox.h"

#include <functional>

typedef std::function<void(VisiblePoint *)> QueryCallback;

class Grid
{
public:
	virtual ~Grid()
	{}

	virtual void query(const Pos &pos, real r_bound, const QueryCallback &callback) = 0;
};

// a KD tree that contains visible points (as weak refrence)
class KDGrid : public Grid
{
private:
	BoundingBox *box;
	VPPtrList *vps;        // weak ptrs
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
	void query(const Pos &pos, real r_bound, const QueryCallback &callback) override;
};


class NaiveGrid : public Grid
{
private:
	VPPtrList vps;
public:
	explicit NaiveGrid(const VPPtrList &vplist) : vps(vplist)
	{}

	void query(const Pos &pos, real r_bound, const QueryCallback &callback) override;
};


class UniformGrid : public Grid
{
private:
	Pos lower, upper, d;	// d = (upper - lower) / n_grid
	List3D<VPPtrList> grids;	// each grid is a list of vp ptr

	inline Vec<int> mapToGrid(const Pos &pos) const	// floor to index
	{ return {int((pos.x - lower.x) / d.x), int((pos.y - lower.y) / d.y), int((pos.z - lower.z) / d.z)}; }

public:
	explicit UniformGrid(const VPPtrList &vplist);

	void storeToGrid(VisiblePoint *vp);

	void query(const Pos &pos, real r_bound, const QueryCallback &callback) override;

	static const int n_grid;
};


#endif //PHAROSA_KDGRID_H
