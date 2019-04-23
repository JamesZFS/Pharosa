//
// Created by James on 2019/4/10.
//

#ifndef PHAROSA_GEOMETRY_H
#define PHAROSA_GEOMETRY_H

#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Mat.h"
#include "../core/Ray.hpp"


struct Geometry
{
	TransMat mat;    // transform matrix, always do rotation first, then translation

	Geometry() = default;

	Geometry(const Pos &pos_, const ElAg &euler_angles_);

	Geometry(const Geometry &geo) = default;  // copy

//	Geometry(Geometry &&geo) noexcept;	// move todo

	Geometry &operator=(const Geometry &geo) = default;    // copy

//	Geometry &operator=(Geometry &&geo) noexcept;    // move

	virtual ~Geometry() = default;

	// 3D transformation, return *this
	Geometry &place(const Pos &pos_, const ElAg &euler_angles_);

	Geometry &translate(const Pos &delta);

	Geometry &rotate(const ElAg &dea);

	// interfaces:
	virtual void applyTransform() = 0;       // re-apply transform from transform matrix

	/** standard intersection api. todo calculate normal at the same time when intersecting
	 * @param ray
	 * @param t : distance to first intersection point
	 * @return true if intersected else false
	 */
	virtual bool intersect(const Ray &ray, double &t) const = 0;

	// calculate normal vector at surface point x
	virtual Dir normalAt(const Pos &x) const = 0;

	// determine if a point is on the surface of object
	virtual bool hasSurfacePoint(const Pos &x) const = 0;

	// todo code below is dirty
	virtual void translateLocal(const Pos &delta);

	virtual double xMin();

	virtual double xMax();

	virtual double yMin();

	virtual double yMax();

	virtual double zMin();

	virtual double zMax();
};

#endif //PHAROSA_GEOMETRY_H
