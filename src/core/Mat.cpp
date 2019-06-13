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
	real
			cos_a = cosf(ea.alpha), sin_a = sinf(ea.alpha),
			cos_b = cosf(ea.beta), sin_b = sinf(ea.beta),
			cos_g = cosf(ea.gamma), sin_g = sinf(ea.gamma);

	rot = Mat<real>(
			{{
					 {{cos_a * cos_g - sin_a * sin_b * sin_g, -cos_b * sin_a, cos_g * sin_a * sin_b + cos_a * sin_g}},
					 {{cos_g * sin_a + cos_a * sin_b * sin_g, cos_a * cos_b, -cos_a * cos_g * sin_b + sin_a * sin_g}},
					 {{-cos_b * sin_g, sin_b, cos_b * cos_g}}
			 }}
	);
}

TransMat::TransMat(const Vec<real> &tra_, const Mat<real> &rot_) : tra(tra_), rot(rot_)
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
