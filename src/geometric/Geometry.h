//
// Created by James on 2019/4/10.
//

#ifndef PHAROSA_GEOMETRY_H
#define PHAROSA_GEOMETRY_H

//#include "../lib.h"
//#include "../core/Vec.h"
//#include "../core/Ray.hpp"
//#include "../core/Mat.h"
//
//
//struct Geometry
//{
//	Pos pos;        // ref point position in global coordinate system
//	ElAg ea;        // Euler angles
//
//	Geometry();
//
//	Geometry(const Pos &pos_,
//		  	 const ElAg &euler_angles_ = {0, 0, 0});
//
//	virtual ~Geometry() = default;
//
//	// 3D transformation, return *this
//	Geometry &replace(const Pos &pos_, const ElAg &euler_angles_);
//
//	Geometry &translate(const Pos &delta);
//
//	virtual Geometry &rotate(const ElAg &dea);
//
//	// interfaces:
//	virtual void applyTransform() = 0;    // re-apply transform from obj crd to global crd
//
//	/** standard intersection api.
//	 * @param ray
//	 * @param t : distance to first intersection point
//	 * @return true if intersected else false
//	 */
//	virtual bool intersect(const Ray &ray, double &t) const = 0;
//
//	// calculate normal vector at surface point x
//	virtual Dir normalAt(const Pos &x) const = 0;
//
//	// determine if a point is on the surface of object
//	virtual bool hasSurfacePoint(const Pos &x) const = 0;
//
//};

#endif //PHAROSA_GEOMETRY_H
