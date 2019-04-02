//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_FUNCS_HPP
#define RAYTRACKER_FUNCS_HPP

#include "../lib.h"

namespace Funcs
{

// staircase function Real -> [0, 1]
	inline double clamp(double x)
	{ return x < 0 ? 0 : x > 1 ? 1 : x; }

// to unsigned char [0 - 255], using gamma correction
#define GAMMA_COEFF 0.45454545454545 // 1 / 2.2

	inline unsigned char toUchar(double x)
	{ return (unsigned char) (pow(clamp(x), GAMMA_COEFF) * 255); }

#undef GAMMA_COEFF

	inline bool startWith(const String &s, const String &prefix)
	{
		return s.find(prefix) == 0;
	}

	inline bool endsWith(const String &s, const String &suffix)
	{
		return s.rfind(suffix) == (s.length() - suffix.length());
	}
}

#endif //RAYTRACKER_FUNCS_HPP
