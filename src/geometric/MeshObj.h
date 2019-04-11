//
// Created by James on 2019/4/5.
//

//#ifndef PHAROSA_MESHOBJ_H
//#define PHAROSA_MESHOBJ_H
//
//#include "Geometry.h"
//#include "Triangle.h"
//
typedef List<Triangle *> TriangleGroup;
//
//struct MeshObj : Geometry
//{
//	TriangleGroup meshes;    // triangle mesh segments
//
//	MeshObj(const Pos &pos_, const ElAg &euler_angles_);
//
//	void fromObjFile(const String &obj_path, double zoom_ratio = 1.0);    // append mesh segments from a .obj file
//
//	void applyTransform() override;    // apply translation and rotation to all meshes
//
//	bool intersect(const Ray &ray, double &t) const override;
//
//	Dir normalAt(const Pos &x) const override;
//
//	bool hasSurfacePoint(const Pos &x) const override;
//};
//
//#endif //PHAROSA_MESHOBJ_H
