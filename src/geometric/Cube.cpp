//
// Created by James on 2019/4/9.
//

#include "Cube.h"

Cube::Cube(Arr<Dir, 3> &&n_, Arr2D<Pos, 3, 2> &&p_, const Pos &pos_, const ElAg &euler_angles_) :
		Geometry(pos_, euler_angles_), n(n_), p(p_)
{
	applyTransform();
}

void Cube::applyTransform()
{
	for (size_t i = 0; i < 3; ++i) {
		Dir ni = mat.rot * n[i];
		slab[i][0] = InfPlane(ni, mat * p[i][0]);
		slab[i][1] = InfPlane(ni, mat * p[i][1]);
	}
}

// ** core function
bool Cube::intersect(const Ray &ray, double &t) const
{
	double tmax = INF, tmin = -INF, ti_max, ti_min, dn;
	for (const auto &s : slab) {	// todo acc
		dn = ray.dir % s[0].n;	// d.n
		if (fabs(dn) < EPS) { // parallel to some face
			if (s[0].relationWith(ray.org) == s[1].relationWith(ray.org)) return false; //and outside the cube
		}

		// assert: s[0].n == s[1].n
		ti_min = (s[0].p - ray.org) % s[0].n / dn;
		ti_max = (s[1].p - ray.org) % s[1].n / dn;
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
