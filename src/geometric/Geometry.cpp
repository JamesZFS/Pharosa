//
// Created by James on 2019/4/10.
//

#include "Geometry.h"

Geometry::Geometry(const Pos &pos_, const ElAg &euler_angles_) : mat(pos_, euler_angles_)
{
}

//Geometry::Geometry(Geometry &&geo) noexcept : mat(std::move(geo.mat))
//{
//}

Geometry &Geometry::place(const Pos &pos_, const ElAg &euler_angles_)
{
	mat = TransMat(pos_, euler_angles_);
	applyTransform();
	return *this;
}

Geometry &Geometry::translate(const Pos &delta)
{
	mat = TransMat(delta) * mat;	// translation as the last operation
	applyTransform();
	return *this;
}

Geometry &Geometry::rotate(const ElAg &dea)
{
	mat *= TransMat(dea);
	applyTransform();
	return *this;
}

// todo use pure virtual
double Geometry::xMin()
{
	return 0;
}

double Geometry::xMax()
{
	return 0;
}

double Geometry::yMin()
{
	return 0;
}

double Geometry::yMax()
{
	return 0;
}

double Geometry::zMin()
{
	return 0;
}

double Geometry::zMax()
{
	return 0;
}

//Geometry &Geometry::operator=(Geometry &&geo) noexcept
//{
//	mat = std::move(geo.mat);
//	return *this;
//}
