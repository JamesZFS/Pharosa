//
// Created by James on 2019/4/19.
//

#include "Object.h"
#include "../geometric/All.h"

Object::~Object()
{
	delete geo;
}
