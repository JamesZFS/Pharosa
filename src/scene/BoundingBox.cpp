//
// Created by James on 2019/4/28.
//

#include "BoundingBox.h"
#include "../geometric/Triangle.h"

BoundingBox::BoundingBox() : xmin(INF), xmax(-INF), ymin(INF), ymax(-INF), zmin(INF), zmax(-INF)
{

}

// !
BoundingBox::BoundingBox(const ObjectList &triangles) : BoundingBox()
{
	for (auto obj : triangles) {
		auto tr = dynamic_cast<Triangle *>(obj->geo);    // down-casting
		assert(tr != nullptr);
		xmin = min2(xmin, tr->xMin());
		xmax = max2(xmax, tr->xMax());
		ymin = min2(ymin, tr->yMin());
		ymax = max2(ymax, tr->yMax());
		zmin = min2(zmin, tr->zMin());
		zmax = max2(zmax, tr->zMax());
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
