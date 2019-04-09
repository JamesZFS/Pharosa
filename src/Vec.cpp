//
// Created by James on 2019/4/9.
//

#include "Vec.hpp"

// common Pos
const Pos Pos::ORIGIN(0, 0, 0);
// common ElAg
const ElAg ElAg::NONROT(0, 0, 0);

// common colors
const Color
		Color::BLACK(0, 0, 0),
		Color::WHITE(1, 1, 1),
		Color::RED(1, 0, 0),
		Color::GREEN(0, 1, 0),
		Color::BLUE(0, 0, 1),
		Color::YELLOW(1, 0.784, 0);

// common Emission
const Emission
		Emission::NONE(0, 0, 0),
		Emission::GLOW(0.5, 0.5, 0.5),
		Emission::BRIGHT(1.0, 1.0, 1.0),
		Emission::SPLENDID(5.0, 5.0, 5.0);
