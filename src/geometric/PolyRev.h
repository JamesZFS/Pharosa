//
// Created by James on 2019/5/15.
//

#ifndef PHAROSA_POLYREVOLUTION_H
#define PHAROSA_POLYREVOLUTION_H

#include "Geometry.h"
#include "../core/Polynomial.h"
#include "../core/Mat.h"

// revolution solid of polynomial, assuming the revolution is along X-axis
struct PolyRev : Geometry
{
	Polynomial phi, psi;    // parametric eqa (x = phi(u), y^2 + z^2 = psi(u)^2)
	Polynomial psi_2;	// cache psi^2

	TransMat mat;    // transform matrix from global to local

	PolyRev() = default;

	PolyRev(Polynomial &&phi_, Polynomial &&psi_);

	void applyTransform(TransMat mat_) override;

	bool intersect(const Ray &ray, real &t, Intersection &isect) const override;

//	void getNormal(const Pos &pos, Dir &normal) const override;

	static PolyRev *acquire(const Json &json);
};


#endif //PHAROSA_POLYREVOLUTION_H
