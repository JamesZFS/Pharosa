//
// Created by James on 2019/5/15.
//

#include "PolyRevolution.h"
#include "../core/Ray.hpp"
#include "../utils/solvers/NonLinear.h"

//struct Fun0 : NonLinear::BinFun
//{
//	const Polynomial &B0;
//	const double L0, dL0;
//
//	Fun0(const Polynomial &phi, const Ray &ray) : B0(phi), L0(ray.org.x), dL0(ray.dir.x)
//	{}
//
//	double operator()(double x0, double x1) const override
//	{
//		return B0(x0) - (L0 + x1 * dL0);
//	}
//
//	double d0(double x0, double x1) const override
//	{
//		return B0.derivative(x0);
//	}
//
//	double d1(double x0, double x1) const override
//	{
//		return -dL0;
//	}
//};

struct BFun //: NonLinear::BinFun
{
	const Polynomial &F, &G;

	BFun(const Polynomial &F_, const Polynomial &G_) : F(F_), G(G_)    // F(x0) - G(x1)
	{}

	inline double operator()(double x0, double x1) const
	{
		return F(x0) - G(x1);
	}

	inline double d0(double x0, double x1) const
	{
		return F.derivative(x0);
	}

	inline double d1(double x0, double x1) const
	{
		return -G.derivative(x1);
	}
};

struct MFun //: NonLinear::MonoFun
{
	const Polynomial &F;
	const double b;

	MFun(const Polynomial &F_, double b_) : F(F_), b(b_)
	{}

	inline double operator()(double x) const
	{
		return F(x) - b;
	}

	inline double d(double x) const
	{
		return F.derivative(x);
	}
};

PolyRevolution::PolyRevolution(Polynomial &&phi_, Polynomial &&psi_) :
		phi(std::move(phi_)), psi(std::move(psi_)), psi_2(psi * psi)
{

}

//PolyRevolution::PolyRevolution(List<double> &&x_coeffs, List<double> &&y_coeffs) :
//	phi(x_coeffs), psi(y_coeffs)
//{
//
//}


void PolyRevolution::applyTransform(TransMat mat_)
{
	mat = mat_;
}

bool PolyRevolution::intersect(const Ray &ray, double &t) const
{
	Ray ray_local(mat | ray.org, mat.rot | ray.dir);    // to local todo
	Polynomial
			L0(ray_local.org.x, ray_local.dir.x),
			L1(ray_local.org.y, ray_local.dir.y),
			L2(ray_local.org.z, ray_local.dir.z);
	L1 = L1 * L1 + L2 * L2;
	BFun f0(phi, L0);
	BFun f1(psi_2, L1);
	double u;
	return NonLinear::Solve2DTol(f0, f1, u = Funcs::randf(1.0), t = 0.0, 1e-2) && 0 <= u && u <= 1 && t > EPS;  // Newton Iteration
}

Dir PolyRevolution::normalAt(const Pos &x) const
{
//	return {0, 0, 1};
	Pos x_local = mat | x;
	MFun f(phi, x_local.x);
	double u, cos_v, sin_v, psi_u, phi_p_u;
	assert(NonLinear::SolveTol(f, u = Funcs::randf(1.0)));
	psi_u = psi(u);
	phi_p_u = phi.derivative(u);
	cos_v = x_local.y / psi_u;
	sin_v = x_local.z / psi_u;
	return mat.rot * Pos(-psi.derivative(u), phi_p_u * cos_v, phi_p_u * sin_v);    // transform back todo ??
}

void PolyRevolution::getUV(const Pos &pos, double &u, double &v)
{
	Pos x_local = mat | pos;
	MFun f(phi, x_local.x);
	double cos_v, sin_v, psi_u;
	assert(NonLinear::SolveTol(f, u = Funcs::randf(1.0)));
	psi_u = psi(u);
	cos_v = x_local.y / psi_u;
	sin_v = x_local.z / psi_u;
	v = atan2(sin_v, cos_v);
}
