//
// Created by James on 2019/4/23.
//

#include <algorithm>

namespace Parsing
{
	inline String lowerStr(const String &s)
	{
		String t;
		std::transform(s.begin(), s.end(), std::back_inserter(t), tolower);
		return t;
	}

	inline String &lowerStr_(String &s)	// lower in place
	{
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}
}

namespace Map
{
	struct Case_Free_Comp
	{
		bool operator()(const String &s1, const String &s2)
		{
			return Parsing::lowerStr(s1) < Parsing::lowerStr(s2);
		}
	};

	std::map<const String, const RGB *, Case_Free_Comp> str_to_rgb = {
			{"black",    &Color::BLACK},
			{"white",    &Color::WHITE},
			{"red",      &Color::RED},
			{"R",        &Color::RED},
			{"green",    &Color::GREEN},
			{"G",        &Color::GREEN},
			{"blue",     &Color::BLUE},
			{"B",        &Color::BLUE},
			{"yellow",   &Color::YELLOW},
			{"brown",    &Color::BROWN},
			{"none",     &Emission::NONE},
			{"glow",     &Emission::GLOW},
			{"bright",   &Emission::BRIGHT},
			{"splendid", &Emission::SPLENDID},
	};

	std::map<const String, const Material::ReflType, Case_Free_Comp> str_to_reft = {
			{"DIFF",       Material::DIFF},
			{"diffusive",  Material::DIFF},
			{"SPEC",       Material::SPEC},
			{"specular",   Material::SPEC},
			{"REFR",       Material::REFR},
			{"refractive", Material::REFR},
			{"NONE",       Material::NONE},
	};
}
