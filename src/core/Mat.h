//
// Created by James on 2019/4/11.
//

#ifndef PHAROSA_TRANSMAT_H
#define PHAROSA_TRANSMAT_H

#include "../Pharosa.h"
#include "Vec.h"

template<typename T = real>
struct Mat    // 3D matrix
{
	Arr2D<T, 3, 3> el;

	Mat();	// init a 3x3 zero matrix

	Mat(Arr2D<T, 3, 3> &&a) noexcept;	// move from array

	Mat(T k);    // diag

	Mat(const Mat &b) = default;    // copy

	Mat(Mat &&b) noexcept;	// move

	void assignHorz(const Pos &a0, const Pos &a1, const Pos &a2); 	// assign horizontal vectors

	void assignVert(const Pos &a0, const Pos &a1, const Pos &a2);	// assign vertical vectors

	void reset();	// reset to zero

	Mat<T> &operator=(const Mat &b) = default;    // copy

	Mat<T> &operator=(Mat &&b) noexcept;    // move

	inline T at(size_t i, size_t j) const    // getter
	{ return el.at(i).at(j); }

	Mat<T> operator+(const Mat<T> &B) const;    // pls mul

	Mat<T> operator+=(const Mat<T> &B);    // pls mul in place

	Mat<T> operator*(const Mat<T> &B) const;   // mat mul

	Mat<T> &operator*=(const Mat<T> &B);    // mat mul in place

	Mat<T> operator*(T k) const;    // mul number

	inline Vec<T> operator*(const Vec<T> &b) const;    // mul vector

	inline Vec<T> operator|(const Vec<T> &b) const;		// transpose mul vector

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
	Mat<real> rot;    // rotation matrix

	TransMat();			// identical transformation

	TransMat(const Pos &delta);    // pure translate

	TransMat(const ElAg &ea);    	// pure rotate: Rz.Rx.Ry

	TransMat(const Pos &delta, const ElAg &ea);	// rotate then translate

	TransMat(const Vec<real> &tra_, const Mat<real> &rot_);

	TransMat(const Json &json);	// init from json

	TransMat(const TransMat &mat) = default;	// copy

	TransMat(TransMat &&mat) noexcept;	// move

	TransMat &operator=(const TransMat &mat) = default;	// copy

	TransMat &operator=(TransMat &&mat) noexcept;	// copy

	TransMat &operator*=(const TransMat &B);    // mat mul

	TransMat operator*(const TransMat &B) const;    // mat mul

	inline Vec<real> operator*(const Vec<real> &x) const    // apply transform to a vec
	{ return tra + rot * x; }

	inline Vec<real> operator|(const Vec<real> &x) const 	// inverse transform
	{ return rot | (x - tra); }

	void report() const;
};


#endif //PHAROSA_TRANSMAT_H
