//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_LIB_H
#define PHAROSA_LIB_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <omp.h>

#define INF 1e20
#define EPS 1e-4

using std::printf;

template<typename T>
using List = std::vector<T>;

template<typename T, size_t n>
using Arr = std::array<T, n>;

template<typename T, size_t n0, size_t n1>
using Arr2D = Arr<Arr<T, n1>, n0>;

using String = std::string;

typedef char Buffer[256];

template <typename T>
inline T min2(const T &a, const T &b)
{ return std::min(a, b); }

template <typename T>
inline T max2(const T &a, const T &b)
{ return std::max(a, b); }

template <typename T>
inline T min3(const T &a, const T &b, const T &c)
{ return std::min(std::min(a, b), c); }

template <typename T>
inline T max3(const T &a, const T &b, const T &c)
{ return std::max(std::max(a, b), c); }

#define __DEV_STAGE__

#ifdef __DEV_STAGE__
	#define debug(...) printf(__VA_ARGS__)              // for dev
	//#define prompt(...) fprintf(stderr, __VA_ARGS__)    // show progressbar info

	// global params for debugging
	extern size_t __counter__;

#else	// when release
	#undef assert
	#define assert(...)
	#define printf(...)

#endif

#define warn(x) std::cerr << x << std::endl         // show error
#define message(x) std::cout << x << std::endl        // show message

#endif //PHAROSA_LIB_H
