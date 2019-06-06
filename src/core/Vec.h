//
// Created by think on 2019/5/1.
//

#ifndef PHAROSA_VEC_H
#define PHAROSA_VEC_H

#include "../Pharosa.h"
#include "../utils/funcs.hpp"
#include "../utils/parsers/json_fwd.hpp"

// vector definition
template<typename T = real>
struct Vec
{
	T x, y, z;

	Vec(T x_ = 0, T y_ = 0, T z_ = 0) : x(x_), y(y_), z(z_)
	{}

	inline real operator[](size_t i) const
	{
		assert(i < 3);
		return *(&x + i);
	}

	inline bool hasPositivePart() const
	{ return x > 0 || y > 0 || z > 0; }

	inline bool operator==(const Vec &b) const
	{ return (*this - b).sqr() < EPS; }

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
	{ return sqrtf(x * x + y * y + z * z); }

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

	inline void putToArray(T *dst, size_t step = 1) const    // output to `dst`: an array of T
	{
		*(dst) = x;
		*(dst += step) = y;
		*(dst + step) = z;
	}

	inline void report(bool endl = false) const
	{
		printf("(%5.2f, %5.2f, %5.2f)", x, y, z);
		if (endl) printf("\n");
	}

	inline String toString() const
	{
		Buffer buffer;
		sprintf(buffer, "(%.2f, %.2f, %.2f)", x, y, z);
		return String(buffer);
	}
};

struct ElAg : Vec<real>    // Euler angles (Z - alpha, X - beta, Y - gamma)
{
	real &alpha, &beta, &gamma;

	ElAg(real a_ = 0, real b_ = 0, real g_ = 0) : Vec(a_, b_, g_), alpha(x), beta(y), gamma(z)
	{}    // todo can use cosine, sine cache to boost

	ElAg(const Json &json);    // from json, use degrees

	ElAg(const Vec<real> &obj) : ElAg(obj.x, obj.y, obj.z)    // copy constructor
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

struct Pos : Vec<real>    // 3D coordinate
{
	Pos(real x_ = 0, real y_ = 0, real z_ = 0) : Vec(x_, y_, z_)
	{}

	Pos(const Vec<real> &obj) : Pos(obj.x, obj.y, obj.z)    // copy constructor
	{}

	Pos(const Json &json);

	inline Pos operator-() const
	{ return {-x, -y, -z}; }

	using Vec::operator-;

	inline void rotateAlongX(real theta)
	{
		real y_ = cosf(theta) * y - sinf(theta) * z;
		z = sinf(theta) * y + cosf(theta) * z;
		y = y_;
	}

	inline void rotateAlongY(real theta)
	{
		real z_ = cosf(theta) * z - sinf(theta) * x;
		x = sinf(theta) * z + cosf(theta) * x;
		z = z_;
	}

	inline void rotateAlongZ(real theta)
	{
		real x_ = cosf(theta) * x - sinf(theta) * y;
		y = sinf(theta) * x + cosf(theta) * y;
		x = x_;
	}

	Pos &rotate(const ElAg &ea);    // Euler rotation, in place. Z -> X -> Y

	inline Pos getCoordAs(const Pos &e1, const Pos &e2, const Pos &e3)    // coordinate transform
	{
		return {
				e1.x * x + e1.y * y + e1.z * z,
				e2.x * x + e2.y * y + e2.z * z,
				e3.x * x + e3.y * y + e3.z * z,
		};
	}

	static inline Pos random(real lower = 0.0, real upper = 1.0)
	{ return {Funcs::randf(lower, upper), Funcs::randf(lower, upper), Funcs::randf(lower, upper)}; }

	static const Pos ORIGIN;
};

struct Dir : Pos        // direction, unitized vector
{
	Dir() = default;

	Dir(real x_, real y_, real z_) : Pos(x_, y_, z_)    // x, y, z should explicitly assigned
	{
		assert(x != 0 || y != 0 || z != 0);
//		if (x == 0 && y == 0 && z == 0) {
//			z = 1;
//			x = 1;
//			y = 1;
//			__print_cnt__++;
//		}
		this->unitize();
	}

	Dir(const Json &json) : Pos(json)
	{
		assert(x != 0 || y != 0 || z != 0);
		this->unitize();
	}

	Dir(const Vec<real> &obj) : Dir(obj.x, obj.y, obj.z)    // copy constructor
	{}

	Dir(const Dir &obj) = default;    // copy constructor

	inline Dir &unitize()    // to unit vector
	{
		*this /= sqrtf(x * x + y * y + z * z);
		return *this;
	}

	inline void getOrthogonalBasis(Dir &ex, Dir &ey) const   // get orthogonal axis (ex, ey) from ez
	{
		ex = (fabsf(x) > .1 ? Pos(0, 1, 0) : Pos(1, 0, 0)) ^ *this; // .1 is the max threshold value for ez.x
		ey = *this ^ ex;
		ex.unitize();
		ey.unitize();
	}

	// get euler angles according to vector, assuming original dir of this is (0, 0, 1)
	inline ElAg getEulerAngles() const
	{ return {atan2f(y, x), 0, atan2f(sqrtf(x * x + y * y), z)}; }

	const static Dir X_AXIS, Y_AXIS, Z_AXIS;
};

struct RGB : Vec<real>        // RGB Vector
{
	real &r, &g, &b;

	RGB(real r_ = 0, real g_ = 0, real b_ = 0) : Vec(r_, g_, b_), r(x), g(y), b(z)
	{}

	RGB(const Json &json);

	RGB(const Vec<real> &obj) : RGB(obj.x, obj.y, obj.z)    // copy constructor
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

	inline RGB &clamp()    // to [0, 1]
	{
		r = Funcs::clamp(r);
		g = Funcs::clamp(g);
		b = Funcs::clamp(b);
		return *this;
	}

	static const RGB BLACK, WHITE, RED, GREEN, BLUE, YELLOW, BROWN;
	static const RGB NONE, GLOW, BRIGHT, SPLENDID;
};

// 2D float vector
struct Vec2f
{
	real x, y;

	Vec2f() : x(0), y(0) {}

	Vec2f(real x_, real y_) : x(x_), y (y_) {}

	inline real operator[](size_t i) const
	{
		assert(i < 2);
		return *(&x + i);
	}
};

typedef RGB Color;            // intrinsic color of an object
typedef RGB Emission;        // RGB emission of an object
using Vec3f = Vec<double>;

#endif //PHAROSA_VEC_H
