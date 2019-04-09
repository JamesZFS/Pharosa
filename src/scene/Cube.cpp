//
// Created by James on 2019/4/9.
//

#include "Cube.h"

Cube::Cube(const Dir n_[3], const Pos p_[3][2], const Pos &pos_, const Color &color_, const Emission &emission_,
		   const ElAg &euler_angles_, Object::ReflType refl_type_) :
		   Object(pos_, color_, emission_, euler_angles_, refl_type_)
{
	for (int i = 0; i < 3; ++i) {
		p[i][0] = p_[i][0];	// cache
		p[i][1] = p_[i][1];
		slab[i][0] = InfPlane(n_[i], pos + p[i][0], color, emi, reft);	// notice slab.pos is global crd
		slab[i][1] = InfPlane(n_[i], pos + p[i][1], color, emi, reft);
	}
}

void Cube::applyTransform()
{
	for (int i = 0; i < 3; ++i) {
		slab[i][0].replace(pos + p[i][0], ea);	// todo problematic ea
	}
}

// ** core function
bool Cube::intersect(const Ray &ray, double &t) const
{
	double tmax = INF, tmin = -INF, ti_max, ti_min, dn;
	for (const auto &s : slab) {
		dn = ray.dir % s[0].n;	// d.n
		if (fabs(dn) < EPS) continue;	// no intersection

		// assert: s[0].n == s[1].n
		ti_min = (s[0].pos - ray.org) % s[0].n / dn;
		ti_max = (s[1].pos - ray.org) % s[1].n / dn;
		if (ti_max < ti_min) std::swap(ti_min, ti_max);

		tmin = std::max(tmin, ti_min);
		tmax = std::min(tmax, ti_max);
	}
	return ((t = tmin) < tmax	// intersect at front?
			? (tmin < EPS ? ((t = tmax) > EPS)	// intersect at back?
						  : true)
			: false);
}

Dir Cube::normalAt(const Pos &x) const
{
	for (const auto &p :slab) {
		if (p[0].hasSurfacePoint(x) || p[1].hasSurfacePoint(x)) return p[0].n;
	}
	assert(false);	// todo
	return {};
}

bool Cube::hasSurfacePoint(const Pos &x) const
{
	for (const auto &p :slab) {
		if (p[0].hasSurfacePoint(x) || p[1].hasSurfacePoint(x)) return true;
	}
	return false;
}
