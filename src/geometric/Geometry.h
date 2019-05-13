//
// Created by James on 2019/4/10.
//

#ifndef PHAROSA_GEOMETRY_H
#define PHAROSA_GEOMETRY_H

#include "../defs.h"
#include "../core/Vec.h"
#include "../core/Mat.h"
#include "../core/Ray.hpp"


// abstract class
struct Geometry
{
	Geometry() = default;

	virtual ~Geometry() = default;

	// interfaces:
	virtual void applyTransform(TransMat mat) = 0;   // apply transform from transform matrix

	/** standard intersection api. todo calculate normal at the same time when intersecting
	 * @param ray
	 * @param t : distance to first intersection point
	 * @return true if intersected else false
	 */
	virtual bool intersect(const Ray &ray, double &t) const = 0;

	// calculate normal vector at surface point x
	virtual Dir normalAt(const Pos &x) const = 0;

	// get texture coordinate
	virtual void getUV(const Pos &pos, double &u, double &v) = 0;

	// todo code below is dirty
	// determine if a point is on the surface of object
	virtual bool hasSurfacePoint(const Pos &x) const = 0;

	static Geometry *acquire(const Json &json);    // new a geometry from json
};

#endif //PHAROSA_GEOMETRY_H
