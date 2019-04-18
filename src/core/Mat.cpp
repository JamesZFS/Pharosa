//
// Created by James on 2019/4/11.
//

#include "Mat.h"

TransMat::TransMat() : tra(Pos::ORIGIN), rot(1)
{
}

TransMat::TransMat(const Pos &delta) : tra(delta), rot(1)
{
}

TransMat::TransMat(const ElAg &ea) : TransMat(Pos::ORIGIN, ea)
{
}

TransMat::TransMat(const Pos &delta, const ElAg &ea) : tra(delta)
{
	double
			cos_a = cos(ea.alpha), sin_a = sin(ea.alpha),
			cos_b = cos(ea.beta), sin_b = sin(ea.beta),
			cos_g = cos(ea.gamma), sin_g = sin(ea.gamma);

	rot.el = new double[3][3]
			{{cos_a * cos_g - sin_a * sin_b * sin_g, -cos_b * sin_a, cos_g * sin_a * sin_b + cos_a * sin_g},
			 {cos_g * sin_a + cos_a * sin_b * sin_g, cos_a * cos_b,  -cos_a * cos_g * sin_b + sin_a * sin_g},
			 {-cos_b * sin_g,                        sin_b,          cos_b * cos_g}};
}

TransMat::TransMat(const Vec<double> &tra_, const Mat<double> &rot_) : tra(tra_), rot(rot_)
{
}

TransMat &TransMat::operator*=(const TransMat &B)
{
	tra += rot * B.tra;
	rot *= B.rot;
	return *this;
}

TransMat TransMat::operator*(const TransMat &B) const
{
	return {tra + rot * B.tra, rot * B.rot};
}

void TransMat::report() const
{
	tra.report(true);
	rot.report();
	debug("\n");
}

TransMat::TransMat(TransMat &&mat) noexcept : tra(mat.tra), rot(std::move(mat.rot))
{
}

TransMat &TransMat::operator=(TransMat &&mat) noexcept
{
	tra = mat.tra;
	rot = std::move(mat.rot);
	return *this;
}
