//
// Created by James on 2019/4/10.
//

#ifndef PHAROSA_GEOMETRY_H
#define PHAROSA_GEOMETRY_H

#include "../Pharosa.h"
#include "../core/Vec.h"
#include "../core/Mat.h"
#include "../core/Ray.hpp"

struct Intersection;

// abstract class
struct Geometry
{
	enum Type
	{
		SPHERE, TRIANGLE, CUBE, INFPLANE, POLYREV, BEZIERREV
	};

	virtual ~Geometry() = default;

	virtual Type type() const = 0;

	// interfaces:
	virtual void applyTransform(TransMat mat) = 0;   // apply transform from transform matrix

	/** standard intersection api. update first intersection
	 * input:
	 * @param ray
	 * @param t : current minimal time
	 * @param isect : current intersection
	 * output:
	 * @param t : distance to first intersection point
	 * @param isect : intersection info
	 * @return true if intersected ahead of t else false
	 */
	virtual bool intersect(const Ray &ray, real &t, Intersection &isect) const = 0;

	// calculate n vector at surface point x
	virtual void getNormal(const Pos &pos, Dir &normal) const
	{}

	// get texture coordinate
	virtual void getUV(const Pos &pos, real &u, real &v) const
	{}

	static Geometry *acquire(const Json &json);    // new a geometry from json

	/// todo max min xyz in another class
};

#endif //PHAROSA_GEOMETRY_H
