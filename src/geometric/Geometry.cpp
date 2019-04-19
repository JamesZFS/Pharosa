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
	warn("Geometry::xMin() not implemented!");
	exit(1);
}

double Geometry::xMax()
{
	warn("Geometry::xMax() not implemented!");
	exit(1);
}

double Geometry::yMin()
{
	warn("Geometry::yMin() not implemented!");
	exit(1);
}

double Geometry::yMax()
{
	warn("Geometry::yMax() not implemented!");
	exit(1);
}

double Geometry::zMin()
{
	warn("Geometry::zMin() not implemented!");
	exit(1);
}

double Geometry::zMax()
{
	warn("Geometry::zMax() not implemented!");
	exit(1);
}

void Geometry::translateLocal(const Pos &delta)
{
	warn("Geometry::translateLocal() not implemented!");
	exit(1);
}

//Geometry &Geometry::operator=(Geometry &&geo) noexcept
//{
//	mat = std::move(geo.mat);
//	return *this;
//}
