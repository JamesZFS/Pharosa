////
//// Created by James on 2019/4/19.
////
//
//#ifndef PHAROSA_BOUNDINGBOX_H
//#define PHAROSA_BOUNDINGBOX_H
//
//#include "../Object.h"
//#include "../../utils/parsers/MeshParser.h"
//
///** abstract class
// * A bounding box is a composition of objects, which should delegates
// * `place, translate, rotate, applyTransform, intersectAny`
// * to all its subordinate objects, and is in charge of deleting them.
// */
//class BoundingBox
//{
//#ifdef __DEV_STAGE__
//public:
//#else
//	protected:
//#endif
//	ObjectList objects;    // object ptrs
//	TransMat mat;    // unified transform matrix
//
//public:
//	BoundingBox() = default;
//
//	BoundingBox(ObjectList &objects_, const Pos &pos_ = {}, const ElAg &euler_angles_ = {});
//
//	BoundingBox(ObjectList &objects_, const TransMat &mat_);
//
//	virtual ~BoundingBox();
//
//	BoundingBox &place(const Pos &pos_, const ElAg &euler_angles_);
//
//	BoundingBox &translate(const Pos &delta);
//
//	BoundingBox &rotate(const ElAg &dea);
//
//	// interfaces:
//	virtual void applyTransform();       // if overriden, should be called at first
//
//	// the derived should call this at last
//	virtual bool intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const;
//
//	virtual void findBounds() = 0; // automatically select a proper bounding box
//};
//
//typedef List<BoundingBox *> BoundingBoxList;
//
//#endif //PHAROSA_BOUNDINGBOX_H
