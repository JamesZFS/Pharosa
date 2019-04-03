//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_LIB_H
#define RAYTRACKER_LIB_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <random>

const double INF = 1e20, EPS = 1e-4;

template<typename T>
using List = std::vector<T>;

using String = std::string;

typedef char Buffer[256];

#define debug(...) printf(__VA_ARGS__)              // for dev
//#define debug(...)								// for release
#define warn(x) std::cerr << x << std::endl         // show error
//#define prompt(...) fprintf(stderr, __VA_ARGS__)    // show progressbar info

#endif //RAYTRACKER_LIB_H
