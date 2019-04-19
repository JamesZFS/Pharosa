//
// Created by James on 2019/4/9.
//

#include "Cube.h"

Cube::Cube(Arr<Dir, 3> &&n_, Arr2D<Pos, 3, 2> &&p_, const Pos &pos_, const ElAg &euler_angles_) :
		Geometry(pos_, euler_angles_), n(n_), p(p_)
{
	applyTransform();    // local to global slabs
}

// init from 3 vertices, ox, oy, oz. by default, the o in local crd sys is (0, 0, 0)
Cube::Cube(Arr<Pos, 3> &&vertices_, const Pos &pos_, const ElAg &euler_angles_) :
		Geometry(pos_, euler_angles_),
		n{{
				  vertices_[0] ^ vertices_[1], // n of oxy todo unitize?
				  vertices_[1] ^ vertices_[2], // oyz
				  vertices_[2] ^ vertices_[0], // ozx
		  }},
		p{{
				  {{Pos::ORIGIN, vertices_[2]}}, // plane oxy, oxy'
				  {{Pos::ORIGIN, vertices_[0]}}, // oyz, oyz'
				  {{Pos::ORIGIN, vertices_[1]}}  // ozx, ozx'
		  }}
{
	applyTransform();    // local to global slabs
}

Cube::Cube(double length, double width, double height, const Pos &pos_, const ElAg &euler_angles_) :
		Geometry(pos_, euler_angles_),
		n{{Dir::Z_AXIS, Dir::X_AXIS, Dir::Y_AXIS}},
		p{{
				  {{Pos::ORIGIN, Pos(0, 0, height)}}, // plane oxy, oxy'
				  {{Pos::ORIGIN, Pos(length, 0, 0)}}, // oyz, oyz'
				  {{Pos::ORIGIN, Pos(0, width, 0)}}   // ozx, ozx'
		  }}
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
	for (const auto &s : slab) {    // todo acc
		dn = ray.dir % s[0].n;    // d.n
		if (fabs(dn) < EPS) { // parallel to some face
			if (s[0].above(ray.org) == s[1].above(ray.org)) return false; //and outside the cube
		}

		// assert: s[0].n == s[1].n
		ti_min = (s[0].p - ray.org) % s[0].n / dn;
		ti_max = (s[1].p - ray.org) % s[1].n / dn;
		if (ti_max < ti_min) std::swap(ti_min, ti_max);

		tmin = std::max(tmin, ti_min);
		tmax = std::min(tmax, ti_max);
	}
	return ((t = tmin) < tmax    // intersect at front?
			? (tmin < EPS ? ((t = tmax) > EPS)    // intersect at back?
						  : true)
			: false);
}

Dir Cube::normalAt(const Pos &x) const
{
	for (const auto &p :slab) {
		if (p[0].hasSurfacePoint(x) || p[1].hasSurfacePoint(x)) return p[0].n;
	}
	assert(false);    // todo
	return {};
}

bool Cube::hasSurfacePoint(const Pos &x) const
{
	for (const auto &p :slab) {
		if (p[0].hasSurfacePoint(x) || p[1].hasSurfacePoint(x)) return true;
	}
	return false;
}

Cube::Cube() : Cube(1, 1, 1)
{
}
