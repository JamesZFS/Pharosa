//
// Created by James on 2019/4/3.
//

#ifndef PHAROSA_PARSER_H
#define PHAROSA_PARSER_H

#include "../../lib.h"
#include "../../core/Vec.h"
#include "../../geometric/All.h"

class JsonParser
{
private:
	// generic:
	static Pos pos;
	static Color color;
	static Emission emission;
	static ElAg euler_angles;
	static Object::ReflType refl_type;

	// sphere:
	static double radius;

	// triangle:
	static Pos points[3];

	// cache:
	static String obj_str;
	static const Object *result;

	static void parseGeneric();

	static void parseSphere();

	static void parseTriangle();

	static void parseObj();

public:
	static void reset();

	static ObjectGroup fromJsonFile(const String &config_path);    // remember to use std::move
};

#endif //PHAROSA_PARSER_H
