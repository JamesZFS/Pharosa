//
// Created by James on 2019/4/9.
//

#include "Cube.h"

// init from 3 basis (ox, oy, oz. by default) and left-bottom-front most point pos
Cube::Cube(const Pos &ox, const Pos &oy, const Pos &oz, const Pos &o)
{
	Arr<Dir, 3> n{ox ^ oy, oy ^ oz, oz ^ ox};
	slab = {{
					{{InfPlane(n[0], o), InfPlane(n[0], o + oz)}},    // plane oxy, oxy'
					{{InfPlane(n[1], o), InfPlane(n[1], o + ox)}},    // oyz, oyz'
					{{InfPlane(n[2], o), InfPlane(n[2], o + oy)}},    // ozx, ozx'
			}};
}

// init an orthogonal cube
Cube::Cube(double length, double width, double height, const Pos &pos) :
		slab{{
					 {{InfPlane(Dir::Z_AXIS, pos), InfPlane(Dir::Z_AXIS, pos + Pos(0, 0, height))}}, // plane oxy, oxy'
					 {{InfPlane(Dir::X_AXIS, pos), InfPlane(Dir::X_AXIS, pos + Pos(length, 0, 0))}}, // oyz, oyz'
					 {{InfPlane(Dir::Y_AXIS, pos), InfPlane(Dir::Y_AXIS, pos + Pos(0, width, 0))}},  // ozx, ozx'
			 }}
{
}

// init a unit cube
Cube::Cube() : Cube(1, 1, 1)
{
}

void Cube::applyTransform(TransMat mat)
{
	for (auto &s : slab) {
		s[0].applyTransform(mat);
		s[1].applyTransform(mat);
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
		ti_min = -(s[0].D + ray.org % s[0].n) / dn;
		ti_max = -(s[1].D + ray.org % s[1].n) / dn;
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
