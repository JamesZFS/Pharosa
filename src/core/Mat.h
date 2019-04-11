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

	~Mat();

	Mat<T> &operator=(const Mat &b);    // copy

	inline T at(unsigned int i, unsigned int j) const    // getter
	{ return el[i][j]; }

	Mat<T> operator+(Mat<T> B) const;    // pls mul

	Mat<T> operator+=(Mat<T> B);    // pls mul in place

	Mat<T> operator*(Mat<T> B) const;   // mat mul

	Mat<T> &operator*=(Mat<T> B);    // mat mul in place

	Mat<T> operator*(T k) const;    // mul number

	inline Vec<T> operator*(Vec<T> b) const;    // mul vector

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

	TransMat();

	TransMat(const Pos &delta);    // translate

	TransMat(const ElAg &ea);    // init from ea: Rz.Rx.Ry

	TransMat(const Pos &delta, const ElAg &ea);

	TransMat(const Vec<double> &tra_, const Mat<double> &rot_);

	TransMat &operator*=(TransMat B);    // mat mul

	TransMat operator*(TransMat B) const;    // mat mul

	inline Vec<double> operator*(Vec<double> x) const    // apply transform to a vec
	{ return tra + rot * x; }

	void report() const;
};


#endif //PHAROSA_TRANSMAT_H
