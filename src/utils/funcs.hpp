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
#define C_GAMMA 0.45454545454545 // 1 / 2.2
#define C_1_GAMMA 2.2 // 2.2 / 1

	inline unsigned int gammaCorrection(double x)
	{ return (unsigned int) (pow(clamp(x), C_GAMMA) * 255); }

	inline double inverseGammaCorrection(unsigned int y)
	{ return pow(y * 1.0 / 255, C_1_GAMMA); }

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

	inline void toLower(String &s)
	{
//		transform(s.begin(), s.end(), s.begin(), ::tolower); todo
	}

	inline void toUpper(String &s)
	{
//		transform(s.begin(), s.end(), s.begin(), ::toupper);
	}
}

#endif //RAYTRACKER_FUNCS_HPP
