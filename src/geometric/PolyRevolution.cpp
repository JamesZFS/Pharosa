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

struct Fun : NonLinear::BinFun
{
	const Polynomial &F, &G;

	Fun(const Polynomial &F_, const Polynomial &G_) : F(F_), G(G_)	// F(x0) - G(x1)
	{}

	double operator()(double x0, double x1) const override
	{
		return F(x0) - G(x1);
	}

	double d0(double x0, double x1) const override
	{
		return F.derivative(x0);
	}

	double d1(double x0, double x1) const override
	{
		return -G.derivative(x1);
	}
};

PolyRevolution::PolyRevolution(Polynomial &&phi_, Polynomial &&psi_) :
	phi(std::move(phi_)), psi(std::move(psi_)), psi_2(psi * psi)
{
}

void PolyRevolution::applyTransform(TransMat mat_)
{
	mat = mat_;
}

bool PolyRevolution::intersect(const Ray &ray, double &t) const
{
	Ray ray_local(mat | ray.org, mat | ray.dir);	// to local
	Polynomial L0, L1, L2;
	L0.set(0, ray_local.org.x), L0.set(1, ray_local.dir.x);
	L1.set(0, ray_local.org.y), L1.set(1, ray_local.dir.y);
	L2.set(0, ray_local.org.z), L2.set(1, ray_local.dir.z);
	Fun f0(phi, L0);
	Fun f1(psi_2, L1 * L1 + L2 * L2);
	double u = Funcs::randf(1.0);	// initial value
	t = 0.0;
	return NonLinear::Solve2DTol(f0, f1, u, t);	// Newton Iteration for solving
}

Dir PolyRevolution::normalAt(const Pos &x) const
{
	return Dir();
}

void PolyRevolution::getUV(const Pos &pos, double &u, double &v)
{

}
