//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_FUNCS_HPP
#define PHAROSA_FUNCS_HPP

#include "../defs.h"
#include <random>


namespace Funcs
{
	// random engine
	extern std::default_random_engine generator;
	extern std::uniform_real_distribution<double> uniform_distribution;
	extern std::normal_distribution<double> normal_distribution;

	inline double randf() // random float [0.0, 1.0)
	{
		return uniform_distribution(generator);
	}

	inline double randf(double upper) // random float [0.0, upper)
	{
		return upper * uniform_distribution(generator);
	}

	inline double randf(double lower, double upper) // [lower, upper)
	{
		return lower + (upper - lower) * uniform_distribution(generator);
	}

	inline double randfNormal()    // 0-1 normal dist
	{
		return normal_distribution(generator);
	}

	inline double randfNormal(double mu, double sigma)
	{
		return mu + sigma * normal_distribution(generator);
	}

	// staircase function Real -> [0, 1]
	inline double clamp(double x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x;
	}

	inline int sgn(double x)
	{
		return (x > EPS ? 1 : (x < -EPS ? -1 : 0));
	}

	// to unsigned char [0 - 255], using gamma correction
#define C_GAMMA 0.45454545454545 // 1 / 2.2
#define C_1_GAMMA 2.2 // 2.2 / 1

	inline size_t gammaCorrection(double x)
	{
		return (size_t) (pow(clamp(x), C_GAMMA) * 255);
	}

	inline double inverseGammaCorrection(size_t y)
	{
		return pow(y * 1.0 / 255, C_1_GAMMA);
	}

#undef C_GAMMA
#undef C_1_GAMMA

	inline bool startWith(const String &s, const String &prefix)
	{
		return s.find(prefix) == 0;
	}

	inline bool endsWith(const String &s, const String &suffix)
	{
		auto idx = s.rfind(suffix);
		return idx != String::npos && idx == (s.length() - suffix.length());
	}
}

#endif //PHAROSA_FUNCS_HPP
