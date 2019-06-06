//
// Created by James on 2019/5/15.
//

#ifndef PHAROSA_POLYNOMIAL_H
#define PHAROSA_POLYNOMIAL_H

#include "../Pharosa.h"

class Polynomial
{
private:
	size_t n;	// order of polynomial
	List<real> a;	// coeffs
	List<real> d;	// derivatives

	void compDerivatives();

	void extendTo(size_t order);	// append 0s

	void trim();	// trim 0s at the end

public:
	Polynomial();

	explicit Polynomial(size_t order);	// f(x) = 0

	Polynomial(List<real> &&coeffs);

	Polynomial(const List<real> &coeffs);

	Polynomial(real a0, real a1);

	Polynomial(Polynomial &&other) noexcept;	// move

	inline auto operator[](size_t i) const	// getter
	{ return (0 <= i && i <= n) ? a[i] : 0; }

	void set(size_t i, real k); 	// setter

	real operator()(real x) const;	// return f(x)

	real derivative(real x) const; 	// return f'(x)

	Polynomial &operator=(const Polynomial &other) = default;

	Polynomial operator+(const Polynomial &other) const;

	Polynomial operator-(const Polynomial &other) const;

	Polynomial &operator+=(const Polynomial &other);

	Polynomial &operator-=(const Polynomial &other);

	Polynomial operator*(const Polynomial &other) const;

	Polynomial &operator*=(const Polynomial &other);

	Polynomial operator*(real k) const;

	Polynomial &operator*=(real k);

	void report() const;

	inline size_t order() const
	{ return n; }

	static Polynomial binomial(real a, real b, unsigned char n); // (a + b x)^n  (n at most = 10)
};


#endif //PHAROSA_POLYNOMIAL_H
