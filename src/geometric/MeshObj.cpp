//
// Created by James on 2019/4/5.
//

#include "MeshObj.h"
#include "../utils/Parsers.h"

MeshObj::MeshObj(const Pos &pos_, const Color &color_, const Emission &emission_, const ElAg &euler_angles_,
				 Object::ReflType refl_type_) : Object(pos_, color_, emission_, euler_angles_, refl_type_)
{}

void MeshObj::fromObjFile(const String &obj_path, double zoom_ratio)
{
	meshes = Parser::fromObjFile(obj_path, zoom_ratio, pos, color, emi, ea, REFR);
}

void MeshObj::applyTransform()
{
	for (Triangle *triangle : meshes) {    // broadcast transformation to all meshes
		triangle->replace(pos, ea);
	}
}

bool MeshObj::intersect(const Ray &ray, double &t) const
{
	// this method should be done by Stage, whose `objects` member can access this->meshes
	return false;
}

Dir MeshObj::normalAt(const Pos &x) const
{
	// this method should be done by Stage, whose `objects` member can access this->meshes
	warn("Error: method `MeshObj::normalAt` should never be called.\n");
	exit(1);
}

bool MeshObj::hasSurfacePoint(const Pos &x) const
{
	for (const auto &triangle: meshes) {
		if (triangle->hasSurfacePoint(x)) return true;
	}
	return false;
}
