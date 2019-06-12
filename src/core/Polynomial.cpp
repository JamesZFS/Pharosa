//
// Created by James on 2019/5/15.
//

#include "Polynomial.h"
#include "../utils/funcs.hpp"

Polynomial::Polynomial() : Polynomial(0)
{
}

Polynomial::Polynomial(size_t order) : n(order), a(order + 1, 0.f), d(order, 0.f)
{
}

Polynomial::Polynomial(List<real> &&coeffs) : n(coeffs.size() - 1), a(std::move(coeffs)), d(n, 0.f)
{
	assert(n >= 0);
	trim();
	compDerivatives();
}

Polynomial::Polynomial(const List<real> &coeffs) : n(coeffs.size() - 1), a(coeffs), d(n, 0.f)
{
	assert(n >= 0);
	trim();
	compDerivatives();
}

Polynomial::Polynomial(real a0, real a1) : n(1), a{a0, a1}, d{a1}
{
}

Polynomial::Polynomial(Polynomial &&other) noexcept : n(other.n), a(std::move(other.a)), d(std::move(other.d))
{
}

real Polynomial::operator()(real x) const
{
	if (n == 0) return a[0];
	real res = 0;
	for (size_t i = n; i > 0; --i) {
		res = res * x + a[i];
	}
	res = res * x + a[0];
	return res;
}

real Polynomial::derivative(real x) const
{
	if (n == 0) return 0;
	if (n == 1) return d[0];
	real res = 0;
	for (size_t i = n - 1; i > 0; --i) {
		res = res * x + d[i];
	}
	res = res * x + d[0];
	return res;
}

void Polynomial::compDerivatives()
{
	for (size_t i = 0; i < n; ++i) {
		d[i] = (i + 1) * a[i + 1];
	}
}

Polynomial Polynomial::operator+(const Polynomial &other) const
{
	Polynomial res(max2(n, other.n));
	for (size_t i = 0; i <= res.n; ++i) {
		res.a[i] = (*this)[i] + other[i];
	}
	res.trim();
	res.compDerivatives();
	return res;
}

Polynomial Polynomial::operator-(const Polynomial &other) const
{
	Polynomial res(max2(n, other.n));
	for (size_t i = 0; i <= res.n; ++i) {
		res.a[i] = (*this)[i] - other[i];
	}
	res.trim();
	res.compDerivatives();
	return res;
}

Polynomial &Polynomial::operator+=(const Polynomial &other)
{
	if (n < other.n) extendTo(other.n);
	for (size_t i = 0; i <= n; ++i) {
		a[i] += other[i];
	}
	trim();
	compDerivatives();
	return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &other)
{
	if (n < other.n) extendTo(other.n);
	for (size_t i = 0; i <= n; ++i) {
		a[i] += other[i];
	}
	trim();
	compDerivatives();
	return *this;
}

void Polynomial::extendTo(size_t order)
{
	for (; n < order; ++n) {
		a.push_back(0);
		d.push_back(0);
	}
}

void Polynomial::trim()
{
	while (n > 0 && a.back() == 0) {
		--n;
		a.pop_back();
		d.pop_back();
	}
}

void Polynomial::report() const
{
	printf("order  = %ld\n", n);
	printf("coeffs = ");
	for (auto ai: a) {
		printf("%.2f ", ai);
	}
	printf("\nders   = ");
	for (auto di: d) {
		printf("%.2f ", di);
	}
	printf("\n");
}

void Polynomial::set(size_t i, real k)
{
	assert(i >= 0);
	if (i > n) extendTo(i);
	a[i] = k;
	if (i > 0) d[i - 1] = i * k;
}

Polynomial Polynomial::operator*(const Polynomial &other) const
{
	Polynomial res(n + other.n);
	for (size_t k = 0; k <= res.n; ++k) {
		for (size_t i = 0; i <= k; ++i) {
			res.a[k] += (*this)[i] * other[k - i];
		}
	}
	res.trim();
	res.compDerivatives();
	return res;
}

Polynomial &Polynomial::operator*=(const Polynomial &other)
{
	return *this = *this * other;
}

Polynomial Polynomial::operator*(real k) const
{
	if (k == 0) return Polynomial();
	Polynomial res(n);
	for (size_t i = 0; i <= n; ++i) {
		res.a[i] = a[i] * k;
	}
	res.compDerivatives();
	return res;
}

Polynomial &Polynomial::operator*=(real k)
{
	if (k == 0) return *this = Polynomial();
	for (auto &ai: a) {
		ai *= k;
	}
	compDerivatives();
	return *this;
}

Polynomial Polynomial::binomial(real a, real b, unsigned char n)
{
	assert(n < 11);
	if (b == 0) return Polynomial(List<real>{powf(a, n)});
	Polynomial res(n);
	if (a == 0) {
		res.a[n] = powf(b, n);
	}
	else {
		real a_n_i = powf(a, n), b_i = 1;    // a^(n-i), b^i
		for (unsigned char i = 0; i <= n; ++i) {
			res.a[i] = Funcs::binomial(n, i) * a_n_i * b_i;
			a_n_i /= a, b_i *= b;
		}
	}
	res.compDerivatives();
	return res;
}
