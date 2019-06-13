//
// Created by James on 2019/4/28.
//

#include "BoundingBox.h"
#include "../geometric/Finite.h"

BoundingBox::BoundingBox() : xmin(0), xmax(0), ymin(0), ymax(0), zmin(0), zmax(0)
{
}

BoundingBox::BoundingBox(const Pos &lower, const Pos &upper) :
		xmin(lower.x), xmax(upper.x),
		ymin(lower.y), ymax(upper.y),
		zmin(lower.z), zmax(upper.z)
{
}

// !
BoundingBox::BoundingBox(ObjectList::const_iterator begin, ObjectList::const_iterator end) : BoundingBox()
{
	if (begin == end) return;
	auto it = begin;
	auto shape = dynamic_cast<Finite *>((*it)->geo);    // down-casting
	assert(shape != nullptr);
	xmin = shape->xMin();
	xmax = shape->xMax();
	ymin = shape->yMin();
	ymax = shape->yMax();
	zmin = shape->zMin();
	zmax = shape->zMax();
	for (++it; it != end; ++it) {
		shape = dynamic_cast<Finite *>((*it)->geo);    // down-casting
		assert(shape != nullptr);
		xmin = min2(xmin, shape->xMin());
		xmax = max2(xmax, shape->xMax());
		ymin = min2(ymin, shape->yMin());
		ymax = max2(ymax, shape->yMax());
		zmin = min2(zmin, shape->zMin());
		zmax = max2(zmax, shape->zMax());
	}
}

BoundingBox::BoundingBox(VPPtrList::const_iterator begin, VPPtrList::const_iterator end)
{
	assert(begin != end);
	auto it = begin;
	auto vp = *it;
	xmin = xmax = vp->pos.x;
	ymin = ymax = vp->pos.y;
	zmin = zmax = vp->pos.z;
	for (++it; it != end; ++it) {
		vp = *it;
		xmin = min2(xmin, vp->pos.x);
		xmax = max2(xmax, vp->pos.x);
		ymin = min2(ymin, vp->pos.y);
		ymax = max2(ymax, vp->pos.y);
		zmin = min2(zmin, vp->pos.z);
		zmax = max2(zmax, vp->pos.z);
	}
}

BoundingBox::BoundingBox(List<Pos>::const_iterator begin, List<Pos>::const_iterator end)
{
	assert(begin != end);
	auto it = begin;
	auto vp = *it;
	xmin = xmax = vp.x;
	ymin = ymax = vp.y;
	zmin = zmax = vp.z;
	for (++it; it != end; ++it) {
		vp = *it;
		xmin = min2(xmin, vp.x);
		xmax = max2(xmax, vp.x);
		ymin = min2(ymin, vp.y);
		ymax = max2(ymax, vp.y);
		zmin = min2(zmin, vp.z);
		zmax = max2(zmax, vp.z);
	}
}

// !!
bool BoundingBox::intersect(const Ray &ray) const
{
	if ((fabsf(ray.dir.x) < EPS && (ray.org.x < xmin || xmax < ray.org.x)) ||
		(fabsf(ray.dir.y) < EPS && (ray.org.y < ymin || ymax < ray.org.y)) ||
		(fabsf(ray.dir.z) < EPS && (ray.org.z < zmin || zmax < ray.org.z)))
		return false;    // parallel to some face and outside the cube (singular case)

	real t_xmin = (xmin - ray.org.x) / ray.dir.x, t_xmax = (xmax - ray.org.x) / ray.dir.x;    // todo boosting
	if (t_xmax < t_xmin) std::swap(t_xmin, t_xmax);

	real t_ymin = (ymin - ray.org.y) / ray.dir.y, t_ymax = (ymax - ray.org.y) / ray.dir.y;
	if (t_ymax < t_ymin) std::swap(t_ymin, t_ymax);

	real t_zmin = (zmin - ray.org.z) / ray.dir.z, t_zmax = (zmax - ray.org.z) / ray.dir.z;
	if (t_zmax < t_zmin) std::swap(t_zmin, t_zmax);

	real t_min = max3(t_xmin, t_ymin, t_zmin), t_max = min3(t_xmax, t_ymax, t_zmax);

	return t_min < t_max && (t_min > EPS || t_max > EPS);
}

// !
BoundingBox::Axis BoundingBox::getLongestAxis() const
{
	real lx = xmax - xmin, ly = ymax - ymin, lz = zmax - zmin;
	return lx >= ly ?
		   (lx >= lz ? X : Z) :
		   (ly >= lz ? Y : Z);
}

void BoundingBox::report() const
{
	debug("BoundingBox::report()\n");
	debug("xmin, xmax = %.2f, %.2f\n", xmin, xmax);
	debug("ymin, ymax = %.2f, %.2f\n", ymin, ymax);
	debug("zmin, zmax = %.2f, %.2f\n", zmin, zmax);
	debug("\n");
}

bool BoundingBox::outsideSphere(const Pos &pos, real r) const
{
	real dx = xmin <= pos.x && pos.x <= xmax ? 0 : min2(fabsf(pos.x - xmin), fabsf(pos.x - xmax));
	real dy = ymin <= pos.y && pos.y <= ymax ? 0 : min2(fabsf(pos.y - ymin), fabsf(pos.y - ymax));
	real dz = zmin <= pos.z && pos.z <= zmax ? 0 : min2(fabsf(pos.z - zmin), fabsf(pos.z - zmax));
	return dx * dx + dy * dy + dz * dz > r * r;
}

bool BoundingBox::insideSphere(const Pos &pos, real r) const
{
	real dx = max2(fabsf(pos.x - xmin), fabsf(pos.x - xmax));
	real dy = max2(fabsf(pos.y - ymin), fabsf(pos.y - ymax));
	real dz = max2(fabsf(pos.z - zmin), fabsf(pos.z - zmax));
	return dx * dx + dy * dy + dz * dz <= r * r;
}

void BoundingBox::extendMargin(real margin)
{
	xmin -= margin;
	ymin -= margin;
	zmin -= margin;
	xmax += margin;
	ymax += margin;
	zmax += margin;
}
