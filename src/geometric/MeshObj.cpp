//
// Created by James on 2019/4/5.
//
//todo

//#include "MeshObj.h"
//#include "../utils/Parsers.h"
//
//MeshObj::MeshObj(const Pos &pos_, const ElAg &euler_angles_) :
//		Geometry(pos_, euler_angles_)
//{
//}
//
//void MeshObj::fromObjFile(const String &obj_path, double zoom_ratio)
//{
//	meshes = Parser::fromObjFile(obj_path, zoom_ratio, mat.tra);
//}
//
//void MeshObj::applyTransform()
//{
//	for (Triangle *triangle : meshes) {    // broadcast transformation to all meshes
//		triangle->place(pos, ea);
//	}
//}
//
//bool MeshObj::intersect(const Ray &ray, double &t) const
//{
//	// this method should be done by Scene, whose `objects` member can access this->meshes
//	return false;
//}
//
//Dir MeshObj::normalAt(const Pos &x) const
//{
//	// this method should be done by Scene, whose `objects` member can access this->meshes
//	warn("Error: method `MeshObj::normalAt` should never be called.\n");
//	exit(1);
//}
//
//bool MeshObj::hasSurfacePoint(const Pos &x) const
//{
//	for (const auto &triangle: meshes) {
//		if (triangle->hasSurfacePoint(x)) return true;
//	}
//	return false;
//}
