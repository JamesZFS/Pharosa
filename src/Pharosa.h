//
// Created by think on 2019/4/1.
//

#ifndef PHAROSA_PHAROSA_H
#define PHAROSA_PHAROSA_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <array>
#include <string>
#include "utils/parsers/json_fwd.hpp"    // forward definition
#include "constants.h"

using std::printf;

using Json = nlohmann::json;

template<typename T>
using List = std::vector<T>;

template<typename T>
using List2D = List<List<T>>;

template<typename T, size_t n>
using Arr = std::array<T, n>;

template<typename T, size_t n0, size_t n1>
using Arr2D = Arr<Arr<T, n1>, n0>;

using String = std::string;

typedef char Buffer[256];

template<typename T>
inline T min2(const T &a, const T &b)
{ return std::min(a, b); }

template<typename T>
inline T max2(const T &a, const T &b)
{ return std::max(a, b); }

template<typename T>
inline T min3(const T &a, const T &b, const T &c)
{ return std::min(std::min(a, b), c); }

template<typename T>
inline T max3(const T &a, const T &b, const T &c)
{ return std::max(std::max(a, b), c); }


// ****************** compile options ******************
#define __DEV_STAGE__
//#define __PRINT_N_ITER__

#ifdef __DEV_STAGE__
#define debug(...) printf(__VA_ARGS__)
#define safe_debug(...) if (__print_cnt__ < 1000) { printf(__VA_ARGS__); ++__print_cnt__; }

// global params for debugging
extern size_t __counter__;
extern real __debug__;
extern size_t __print_cnt__;
extern size_t __max_depth__;

#else	// when release
#undef assert
#define assert(...)

#endif
// ****************** end of options ******************

#define warn(x) std::cerr << x << std::endl         // show error
#define message(x) std::cout << x << std::endl        // show message
#define barInfo(...) fprintf(stderr, __VA_ARGS__); fflush(stderr)    // show progressbar info
#define TERMINATE(...) { fprintf(stderr, __VA_ARGS__); exit(1); }    // exit with error info

#endif //PHAROSA_PHAROSA_H
