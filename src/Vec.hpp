//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_VEC_HPP
#define RAYTRACKER_VEC_HPP

#include "lib.h"

// vector definition
template<typename T>
struct Vec
{
	T x, y, z;

	Vec(T x_ = 0, T y_ = 0, T z_ = 0) : x(x_), y(y_), z(z_)
	{}

	inline Vec operator+(const Vec &b) const
	{ return {x + b.x, y + b.y, z + b.z}; }

	inline Vec &operator+=(const Vec &b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}

	inline Vec operator-(const Vec &b) const
	{ return {x - b.x, y - b.y, z - b.z}; }

	inline Vec &operator-=(const Vec &b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}

	inline Vec operator*(T k) const    // mul number
	{ return {x * k, y * k, z * k}; }

	inline Vec &operator*=(T k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	inline Vec operator/(T k) const    // div number
	{ return {x / k, y / k, z / k}; }

	inline Vec &operator/=(T k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	inline Vec mul(const Vec &b) const    // element-wise multiply
	{ return {x * b.x, y * b.y, z * b.z}; }

	inline T norm()    // Euclidean norm
	{ return sqrt(x * x + y * y + z * z); }

	inline T sqr()    // square
	{ return x * x + y * y + z * z; }

	inline Vec &unitize()    // to unit vector
	{ return *this /= sqrt(x * x + y * y + z * z); }

	inline T operator%(const Vec &b) const    // dot product
	{ return x * b.x + y * b.y + z * b.z; }

	inline Vec operator^(const Vec &b) const    // cross product
	{ return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x}; }

	inline T max() const
	{ return (x >= y && x >= z) ? x : (y >= z) ? y : z; }

	inline T min() const
	{ return (x <= y && x <= z) ? x : (y <= z) ? y : z; }

	inline T mean() const
	{ return (x + y + z) / 3; }

	inline void putToArray(T *dst) const    // output to `dst`: an array of T
	{
		dst[0] = x;
		dst[1] = y;
		dst[2] = z;
	}

	void report(bool endl = false) const
	{
		debug("(%.2f, %.2f, %.2f)", x, y, z);
		if (endl) debug("\n");
	}
};

struct ElAg : public Vec<double>    // Euler angles (alpha, beta, gamma)
{
	double &alpha, &beta, &gamma;

	ElAg(double a_ = 0, double b_ = 0, double g_ = 0) : Vec(a_, b_, g_), alpha(x), beta(y), gamma(z)
	{}    // todo can use cosine, sine cache to boost

	ElAg(const Vec<double> &obj) : ElAg(obj.x, obj.y, obj.z)    // copy constructor
	{}

	ElAg(const ElAg &obj) : ElAg(obj.x, obj.y, obj.z)    // copy constructor
	{}
};

struct Pos : Vec<double>    // 3D coordinate
{
	Pos(double x_ = 0, double y_ = 0, double z_ = 0) : Vec(x_, y_, z_)
	{}

	Pos(const Vec<double> &obj) : Pos(obj.x, obj.y, obj.z)    // copy constructor
	{}

	inline void rotateAlongX(double theta)
	{
		double y_ = cos(theta) * y - sin(theta) * z;
		z = sin(theta) * y + cos(theta) * z;
		y = y_;
	}

	inline void rotateAlongY(double theta)
	{
		double z_ = cos(theta) * z - sin(theta) * x;
		x = sin(theta) * z + cos(theta) * x;
		z = z_;
	}

	inline void rotateAlongZ(double theta)
	{
		double x_ = cos(theta) * x - sin(theta) * y;
		y = sin(theta) * x + cos(theta) * y;
		x = x_;
	}

	Pos &rotate(const ElAg &ea)    // Euler rotation, in place
	{
		rotateAlongX(ea.gamma);
		rotateAlongY(ea.beta);
		rotateAlongZ(ea.alpha);
		return *this;
	}
};

struct Dir : public Vec<double>        // direction data type, should automatically unitize
{
	Dir(double x_ = 0, double y_ = 0, double z_ = 0) : Vec(x_, y_, z_)
	{
		this->unitize();
	}

	Dir(const Vec<double> &obj) : Dir(obj.x, obj.y, obj.z)    // copy constructor
	{}

	Dir(const Dir &obj) : Dir(obj.x, obj.y, obj.z)    // copy constructor
	{}
};

struct Color : public Vec<double>    // RGB Color, range [0, 1]
{
	double &r, &g, &b;

	Color(double r_ = 0, double g_ = 0, double b_ = 0) : Vec(r_, g_, b_), r(x), g(y), b(z)
	{}

	Color(const Vec<double> &obj) : Color(obj.x, obj.y, obj.z)    // copy constructor
	{}

	Color(const Color &obj) : Color(obj.x, obj.y, obj.z)    // copy constructor
	{}
};

typedef Color Emission;

// common colors
#define BLACK {0, 0, 0}
#define WHITE {1, 1, 1}
#define RED {1, 0, 0}
#define GREEN {0, 1, 0}
#define BLUE {0, 0, 1}

// common Pos
#define ORIGIN {0, 0, 0}

// common ElAg
#define NONROT {0, 0, 0}

#endif //RAYTRACKER_VEC_HPP
