//
// Created by James on 2019/4/11.
//

#ifndef PHAROSA_TRANSMAT_H
#define PHAROSA_TRANSMAT_H

#include "../lib.h"
#include "Vec.h"

template<typename T = double>
struct Mat    // 3D matrix
{
	T (*el)[3];

	Mat() = default;

	Mat(const T (&a)[3][3]);

	Mat(T k);    // diag

	Mat(const Mat &b);    // copy

	Mat(Mat &&b) noexcept;	// move

	~Mat();

	Mat<T> &operator=(const Mat &b);    // copy

	Mat<T> &operator=(Mat &&b) noexcept;    // move

	inline T at(unsigned int i, unsigned int j) const    // getter
	{ return el[i][j]; }

	Mat<T> operator+(const Mat<T> &B) const;    // pls mul

	Mat<T> operator+=(const Mat<T> &B);    // pls mul in place

	Mat<T> operator*(const Mat<T> &B) const;   // mat mul

	Mat<T> &operator*=(const Mat<T> &B);    // mat mul in place

	Mat<T> operator*(T k) const;    // mul number

	inline Vec<T> operator*(const Vec<T> &b) const;    // mul vector

	void report() const;
};

#include "Mat.tcc"

/** 3D transform matrix
 *  rotate along origin first, then translate:
 *  [ 1         0(1x3)   ]
 *  [ tra(3x1)  rot(3x3) ]
 */
struct TransMat
{
	Pos tra;    // translation vector
	Mat<double> rot;    // rotation matrix

	TransMat();			// identical transformation

	TransMat(const Pos &delta);    // pure translate

	TransMat(const ElAg &ea);    	// pure rotate: Rz.Rx.Ry

	TransMat(const Pos &delta, const ElAg &ea);	// rotate then translate

	TransMat(const Vec<double> &tra_, const Mat<double> &rot_);

	TransMat(const TransMat &mat) = default;	// copy

	TransMat(TransMat &&mat) noexcept;	// move

	TransMat &operator=(const TransMat &mat) = default;	// copy

	TransMat &operator=(TransMat &&mat) noexcept;	// copy

	TransMat &operator*=(const TransMat &B);    // mat mul

	TransMat operator*(const TransMat &B) const;    // mat mul

	inline Vec<double> operator*(const Vec<double> &x) const    // apply transform to a vec
	{ return tra + rot * x; }

	void report() const;
};


#endif //PHAROSA_TRANSMAT_H
