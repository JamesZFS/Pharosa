//
// Created by James on 2019/4/9.
//

#include "Vec.h"

// common Pos
const Pos Pos::ORIGIN(0, 0, 0);

// common axises
const Dir Dir::X_AXIS(1, 0, 0), Dir::Y_AXIS(0, 1, 0), Dir::Z_AXIS(0, 0, 1);

// common ElAg
const ElAg ElAg::NONROT(0, 0, 0);

// common colors
const Color
		Color::BLACK(0, 0, 0),
		Color::WHITE(0.99, 0.99, 0.99),
		Color::RED(0.99, 0, 0),
		Color::GREEN(0, 0.99, 0),
		Color::BLUE(0, 0, 0.99),
		Color::YELLOW(0.99, 0.784, 0),
		Color::BROWN(0.20, 0.08, 0.02);

// common Emission
const Emission
		Emission::NONE(0, 0, 0),
		Emission::GLOW(0.5, 0.5, 0.5),
		Emission::BRIGHT(1.0, 1.0, 1.0),
		Emission::SPLENDID(5.0, 5.0, 5.0);

Pos &Pos::rotate(const ElAg &ea)
{
	rotateAlongY(ea.gamma);
	rotateAlongX(ea.beta);
	rotateAlongZ(ea.alpha);
	return *this;
}
