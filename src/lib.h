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
#include <string>
#include <random>
#include <omp.h>

#define INF 1e20
#define EPS 1e-4

template<typename T>
using List = std::vector<T>;

template<typename T>
using List2D = List<List<T> >;

using String = std::string;

typedef char Buffer[256];

#define debug(...) printf(__VA_ARGS__)              // for dev
//#define debug(...)								// for release
#define warn(x) std::cerr << x << std::endl         // show error
#define message(x) std::cout << x << std::endl         // show error
//#define prompt(...) fprintf(stderr, __VA_ARGS__)    // show progressbar info
//#define assert(...)	// for release

#endif //PHAROSA_LIB_H
