//
// Created by James on 2019/5/15.
//

#include "PolyRev.h"
#include "../core/Ray.hpp"
#include "../utils/solvers/NonLinear.h"
#include "../scene/Intersection.hpp"

//struct Fun0 : NonLinear::BinFun
//{
//	const Polynomial &B0;
//	const real L0, dL0;
//
//	Fun0(const Polynomial &phi, const Ray &ray) : B0(phi), L0(ray.org.x), dL0(ray.dir.x)
//	{}
//
//	real operator()(real x0, real x1) const override
//	{
//		return B0(x0) - (L0 + x1 * dL0);
//	}
//
//	real d0(real x0, real x1) const override
//	{
//		return B0.derivative(x0);
//	}
//
//	real d1(real x0, real x1) const override
//	{
//		return -dL0;
//	}
//};

struct BFun //: NonLinear::BinFun
{
	const Polynomial &F, &G;

	BFun(const Polynomial &F_, const Polynomial &G_) : F(F_), G(G_)    // F(x0) - G(x1)
	{}

	inline real operator()(real x0, real x1) const
	{
		return F(x0) - G(x1);
	}

	inline real d0(real x0, real x1) const
	{
		return F.derivative(x0);
	}

	inline real d1(real x0, real x1) const
	{
		return -G.derivative(x1);
	}
};

struct MFun //: NonLinear::MonoFun
{
	const Polynomial &F;
	const real b;

	MFun(const Polynomial &F_, real b_) : F(F_), b(b_)
	{}

	inline real operator()(real x) const
	{
		return F(x) - b;
	}

	inline real d(real x) const
	{
		return F.derivative(x);
	}
};

PolyRev::PolyRev(Polynomial &&phi_, Polynomial &&psi_) :
		phi(std::move(phi_)), psi(std::move(psi_)), psi_2(psi * psi)
{

}

//PolyRev::PolyRev(List<real> &&x_coeffs, List<real> &&y_coeffs) :
//	phi(x_coeffs), psi(y_coeffs)
//{
//
//}


void PolyRev::applyTransform(TransMat mat_)
{
	mat = mat_;
}

bool PolyRev::intersect(const Ray &ray, real &t, Intersection &isect) const
{
	Ray ray_local(mat | ray.org, mat.rot | ray.dir);    // to local todo
	Polynomial
			L0(ray_local.org.x, ray_local.dir.x),
			L1(ray_local.org.y, ray_local.dir.y),
			L2(ray_local.org.z, ray_local.dir.z);
	L1 = L1 * L1 + L2 * L2;
	BFun f0(phi, L0);
	BFun f1(psi_2, L1);
	real u = Funcs::randf(1.0), ti = 0.0;
	// Newton Iteration
	if (!NonLinear::Solve2DTol(f0, f1, u, ti, 1e-2) || u < 0 || 1 < u || ti < EPS || ti >= t) return false;

	// update:
	t = ti;
	real
			y = ray_local.org.y + t * ray_local.dir.y, z = ray_local.org.z + t * ray_local.dir.z,
			psi_u = psi(u), phi_p_u = phi.derivative(u),
			cos_v = y / psi_u, sin_v = z / psi_u;

	isect.normal = mat.rot * Pos(-psi.derivative(u), phi_p_u * cos_v, phi_p_u * sin_v);
	isect.u = u;
	isect.v = atan2f(sin_v, cos_v);
	return true;
}

//void PolyRev::getNormal(const Pos &pos, Dir &normal) const
//{
//	Pos x_local = mat | pos;
//	MFun f(phi, x_local.x);
//	real u, cos_v, sin_v, psi_u, phi_p_u;
//	assert(NonLinear::SolveTol(f, u = Funcs::randf(1.0)));	// solve for u
//	psi_u = psi(u);
//	phi_p_u = phi.derivative(u);
//	cos_v = x_local.y / psi_u;
//	sin_v = x_local.z / psi_u;
//	normal = mat.rot * Pos(-psi.derivative(u), phi_p_u * cos_v, phi_p_u * sin_v);    // transform back todo ??
//}
//
//void PolyRev::getUV(const Pos &pos, real &u, real &v)
//{
//	Pos x_local = mat | pos;
//	MFun f(phi, x_local.x);
//	real cos_v, sin_v, psi_u;
//	assert(NonLinear::SolveTol(f, u = Funcs::randf(1.0)));
//	psi_u = psi(u);
//	cos_v = x_local.y / psi_u;
//	sin_v = x_local.z / psi_u;
//	v = atan2f(sin_v, cos_v);
//}
