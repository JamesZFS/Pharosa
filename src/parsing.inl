//
// Created by James on 2019/4/23.
//

#include <algorithm>

namespace Map
{
	struct Case_Free_Comp
	{
		bool operator()(const String &s1, const String &s2)
		{
			String a, b;
			std::transform(s1.begin(), s1.end(), std::back_inserter(a), tolower);
			std::transform(s2.begin(), s2.end(), std::back_inserter(b), tolower);
			return a < b;
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

	std::map<const String, const Object::ReflType, Case_Free_Comp> str_to_material = {
			{"DIFF", Object::DIFF},
			{"diffusive", Object::DIFF},
			{"SPEC", Object::SPEC},
			{"spec", Object::SPEC},
			{"REFR", Object::REFR},
			{"NONE", Object::NONE},
	};
}
