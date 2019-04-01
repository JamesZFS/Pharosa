//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_VEC_HPP
#define RAYTRACKER_VEC_HPP

#include "lib.h"

// vector struct
struct Vec
{
	double x, y, z;                  // position, also color (r,g,b)
	Vec(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_)
	{}

	Vec operator+(const Vec &b) const
	{ return {x + b.x, y + b.y, z + b.z}; }

	Vec &operator+=(const Vec &b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}

	Vec operator-(const Vec &b) const
	{ return {x - b.x, y - b.y, z - b.z}; }

	Vec &operator-=(const Vec &b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}

	Vec operator*(double k) const	// mul number
	{ return {x * k, y * k, z * k}; }

	Vec &operator*=(double k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vec operator/(double k) const	// div number
	{ return {x / k, y / k, z / k}; }

	Vec &operator/=(double k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	Vec mul(const Vec &b) const    // element-wise multiply
	{ return {x * b.x, y * b.y, z * b.z}; }

	double norm()
	{ return sqrt(x * x + y * y + z * z); }

	Vec &unitize()	// to unit vector
	{ return *this /= sqrt(x * x + y * y + z * z); }

	double operator*(const Vec &b) const	// dot product
	{ return x * b.x + y * b.y + z * b.z; }

	Vec operator^(const Vec &b) const    // cross product
	{ return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x}; }

	double max() const
	{ return (x >= y && x >= z) ? x : (y >= z) ? y : z; }

	double mean() const
	{ return (x + y + z) / 3; }

	/*void get_orthogonal_axises(Vec &ei, Vec &ej) const
	{
//		Vec ei = ((fabs(x) > .1 ? Vec(0, 1) : Vec(1)) ^ *this).norm();
		ei = (Vec(1) ^ *this).norm();
		ej = *this ^ ei;
	}*/

	void report(bool endl = false)
	{
		printf("(%.2f, %.2f, %.2f)", x, y, z);
		if (endl) printf("\n");
	}
};

#endif //RAYTRACKER_VEC_HPP
