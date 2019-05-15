//
// Created by James on 2019/5/15.
//

#include "PolyRevolution.h"
#include "../utils/solvers/NonLinear.h"

PolyRevolution::PolyRevolution(Polynomial &&phi_, Polynomial &&psi_) : phi(std::move(phi_)), psi(std::move(psi_))
{
}

void PolyRevolution::applyTransform(TransMat mat_)
{
	mat = mat_;
}

bool PolyRevolution::intersect(const Ray &ray, double &t) const
{

}

Dir PolyRevolution::normalAt(const Pos &x) const
{
	return Dir();
}

void PolyRevolution::getUV(const Pos &pos, double &u, double &v)
{

}
