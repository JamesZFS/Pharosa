//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_VEC_HPP
#define RAYTRACKER_VEC_HPP

#include "lib.h"

// vector struct
template<typename T>
struct Vec
{
	T x, y, z;

	Vec(T x_ = 0, T y_ = 0, T z_ = 0) : x(x_), y(y_), z(z_)
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

	Vec operator*(T k) const    // mul number
	{ return {x * k, y * k, z * k}; }

	Vec &operator*=(T k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vec operator/(T k) const    // div number
	{ return {x / k, y / k, z / k}; }

	Vec &operator/=(T k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	Vec mul(const Vec &b) const    // element-wise multiply
	{ return {x * b.x, y * b.y, z * b.z}; }

	T norm()
	{ return sqrt(x * x + y * y + z * z); }

	Vec &unitize()    // to unit vector
	{ return *this /= sqrt(x * x + y * y + z * z); }

	T operator*(const Vec &b) const    // dot product
	{ return x * b.x + y * b.y + z * b.z; }

	Vec operator^(const Vec &b) const    // cross product
	{ return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x}; }

	T max() const
	{ return (x >= y && x >= z) ? x : (y >= z) ? y : z; }

	T mean() const
	{ return (x + y + z) / 3; }

	/*void get_orthogonal_axises(Vec &ei, Vec &ej) const
	{
		//Vec ei = ((fabs(x) > .1 ? Vec(0, 1) : Vec(1)) ^ *this).norm();
		ei = (Vec(1) ^ *this).norm();
		ej = *this ^ ei;
	}*/

	void report(bool endl = false) const
	{
		debug("(%.2f, %.2f, %.2f)", x, y, z);
		if (endl) debug("\n");
	}
};

typedef Vec<double> Pos;    // 3D coordinate

struct Color : public Vec<double>    // RGB Color, range [0, 1]
{
	Color(double r_ = 0, double g_ = 0, double b_ = 0) : Vec(r_, g_, b_)
	{}

	double r() const
	{ return x; }

	double g() const
	{ return y; }

	double b() const
	{ return z; }
};

struct Dir : public Vec<double>		// direction data type, should automatically unitize
{
	Dir(double x_, double y_, double z_) : Vec(x_, y_, z_)
	{
		this->unitize();
	}
};

// common colors
#define BLACK {0, 0, 0}
#define WHITE {1, 1, 1}
#define RED {1, 0, 0}
#define GREEN {0, 1, 0}
#define BLUE {0, 0, 1}

#endif //RAYTRACKER_VEC_HPP
