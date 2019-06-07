//
// Created by James on 2019/4/9.
//

#include "Cube.h"
#include "../scene/Intersection.h"

// init from 3 basis (ox, oy, oz. by default) and left-bottom-front most point c
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
Cube::Cube(real length, real width, real height, const Pos &pos) :
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

// !! core function
bool Cube::intersect(const Ray &ray, real &t, Intersection &isect) const
{
	real tmax = INF, tmin = -INF, ti_max, ti_min, dn;
	for (const auto &s : slab) {    // todo acc
		dn = ray.dir % s[0].n;    // d.n
		if (fabsf(dn) < EPS && s[0].above(ray.org) == s[1].above(ray.org)) { // parallel to some face
			return false; //and outside the cube
		}

		// assert: s[0].n == s[1].n
		ti_min = -(s[0].D + ray.org % s[0].n) / dn;
		ti_max = -(s[1].D + ray.org % s[1].n) / dn;
		if (ti_max < ti_min) std::swap(ti_min, ti_max);

		tmin = max2(tmin, ti_min);
		tmax = min2(tmax, ti_max);
	}
	if (tmin > tmax) return false;    // intersect at front?

	real ti;
	ti = tmin > EPS
		 ? tmin
		 : tmax > EPS ? tmax : INF;  // intersect at back?
	if (ti >= t) return false;
	t = ti;    // update
	return true;
}

// !!
void Cube::getNormal(const Pos &pos, Dir &normal) const
{
	for (const auto &p :slab) {
		if (p[0].testPoint(pos) || p[1].testPoint(pos)) {
			normal = p[0].n;
			return;
		}
	}
	TERMINATE("\nCube::getNormal() should not reach the end!\n")
}

