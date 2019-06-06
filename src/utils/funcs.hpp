//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_FUNCS_HPP
#define PHAROSA_FUNCS_HPP

#include "../Pharosa.h"
#include <random>


namespace Funcs
{
	// random engine
	extern std::default_random_engine generator;
	extern std::uniform_real_distribution<real> uniform_distribution;
	extern std::normal_distribution<real> normal_distribution;

	inline real randf() // random float [0.0, 1.0)
	{
		return uniform_distribution(generator);
	}

	inline real randf(real upper) // random float [0.0, upper)
	{
		return upper * uniform_distribution(generator);
	}

	inline real randf(real lower, real upper) // [lower, upper)
	{
		return lower + (upper - lower) * uniform_distribution(generator);
	}

	inline real randfNormal()    // 0-1 normal dist
	{
		return normal_distribution(generator);
	}

	inline real randfNormal(real mu, real sigma)
	{
		return mu + sigma * normal_distribution(generator);
	}

	// staircase function Real -> [0, 1]
	inline real clamp(real x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x;
	}

	inline int sgn(real x)
	{
		return (x > EPS ? 1 : (x < -EPS ? -1 : 0));
	}

	// to unsigned char [0 - 255], using gamma correction
#define C_GAMMA 0.45454545454545f // 1 / 2.2
#define C_1_GAMMA 2.2f // 2.2 / 1

	inline size_t gammaCorrection(real x)
	{
		return (size_t) (powf(clamp(x), C_GAMMA) * 255);
	}

	inline real inverseGammaCorrection(size_t y)
	{
		return powf(y * 1.0f / 255, C_1_GAMMA);
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

	extern real factorial[11];

	inline real binomial(unsigned char n, unsigned char m)	// (n m)
	{
		assert(0 <= n && n < 11 && 0 <= m && m <= n);	// todo
		return factorial[n] / (factorial[m] * factorial[n - m]);
	}
}

#define WITH_PROB(p) (Funcs::randf() < (p))

#endif //PHAROSA_FUNCS_HPP
