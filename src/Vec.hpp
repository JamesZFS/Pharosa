//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_VEC_HPP
#define PHAROSA_VEC_HPP

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
//
//	inline auto &unitize()    // to unit vector
//	{
//		*this /= sqrt(x * x + y * y + z * z);
//		return *this;
//	}
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

	inline ElAg &operator=(const ElAg &obj)    // copy assignment
	{
		alpha = obj.alpha;
		beta = obj.beta;
		gamma = obj.gamma;
		return *this;
	}

	static const ElAg NONROT;
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
		rotateAlongZ(ea.gamma);
		rotateAlongX(ea.beta);
		rotateAlongZ(ea.alpha);
		return *this;
	}

	static const Pos ORIGIN;
};

struct Dir : public Pos        // direction, unitized vector
{
	Dir() = default;

	Dir(double x_, double y_, double z_) : Pos(x_, y_, z_)    // x, y, z should explicitly assigned
	{
		assert(x != 0 || y != 0 || z != 0); // todo?
		this->unitize();
	}

	Dir(const Vec<double> &obj) : Dir(obj.x, obj.y, obj.z)    // copy constructor
	{}

	Dir(const Dir &obj) = default;    // copy constructor

	inline Dir &unitize()    // to unit vector
	{
		*this /= sqrt(x * x + y * y + z * z);
		return *this;
	}

	void getOrthogonalBasis(Dir &ex, Dir &ey) const   // get orthogonal axis (ex, ey) from ez
	{
		ex = (fabs(x) > .1 ? Pos(0, 1, 0) : Pos(1, 0, 0)) ^ *this; // .1 is the max threshold value for ez.x
		ey = *this ^ ex;
		ex.unitize();
		ey.unitize();
	}
};

struct RGB : public Vec<double>        // RGB Vector
{
	double &r, &g, &b;

	RGB(double r_ = 0, double g_ = 0, double b_ = 0) : Vec(r_, g_, b_), r(x), g(y), b(z)
	{}

	RGB(const Vec<double> &obj) : RGB(obj.x, obj.y, obj.z)    // copy constructor
	{}

	RGB(const RGB &obj) : RGB(obj.x, obj.y, obj.z)    // copy constructor
	{}

	inline RGB &operator=(const RGB &obj)    // copy assignment
	{
		r = obj.r;
		g = obj.g;
		b = obj.b;
		return *this;
	}

	static const RGB BLACK, WHITE, RED, GREEN, BLUE, YELLOW;
	static const RGB NONE, GLOW, BRIGHT, SPLENDID;
};

typedef RGB Color;            // intrinsic color of an object
typedef RGB Emission;        // RGB emission of an object

// common Pos
const Pos Pos::ORIGIN(0, 0, 0);

// common ElAg
const ElAg ElAg::NONROT(0, 0, 0);
#define DEG * 0.0174532925199432957692369076848861271	// degree to rad

// common colors
const Color
		Color::BLACK(0, 0, 0),
		Color::WHITE(1, 1, 1),
		Color::RED(1, 0, 0),
		Color::GREEN(0, 1, 0),
		Color::BLUE(0, 0, 1),
		Color::YELLOW(1, 0.784, 0);

// common Emission
const Emission
		Emission::NONE(0, 0, 0),
		Emission::GLOW(0.5, 0.5, 0.5),
		Emission::BRIGHT(1.0, 1.0, 1.0),
		Emission::SPLENDID(5.0, 5.0, 5.0);

#endif //PHAROSA_VEC_HPP
