//
// Created by James on 2019/5/15.
//

#include "PolyRev.h"
#include "../core/Ray.hpp"
#include "../utils/solvers/NonLinear.h"
#include "../scene/Intersection.h"


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

void PolyRev::applyTransform(const TransMat &mat_)
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

	isect.n = mat.rot * Pos(-psi.derivative(u), phi_p_u * cos_v, phi_p_u * sin_v);
	isect.u = u;
	isect.v = atan2f(sin_v, cos_v);
	return true;
}

void PolyRev::report() const
{
	printf("<Polynomial Revolution>\nphi = ");
	phi.report();
	printf("psi = ");
	psi.report();
	printf("transform:");
	mat.report();
}
