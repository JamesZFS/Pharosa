//
// Created by James on 2019/4/19.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox(ObjectList &objects_, const Pos &pos_, const ElAg &euler_angles_) :
		objects(std::move(objects_)), mat(pos_, euler_angles_)
{
	applyTransform();
}

BoundingBox::BoundingBox(ObjectList &objects_, const TransMat &mat_) :
		objects(std::move(objects_)), mat(mat_)
{
	applyTransform();
}

void BoundingBox::applyTransform()	// will apply the unified transform matrix to all objects
{
#ifdef __DEV_STAGE__
	warn("BoundingBox::applyTransform()\n");
#endif
	for (auto *obj : objects) {
		obj->geo->mat = mat;	// todo use matrix pointer
		obj->geo->applyTransform();
	}
}

BoundingBox &BoundingBox::place(const Pos &pos_, const ElAg &euler_angles_)
{
	mat = TransMat(pos_, euler_angles_);
	applyTransform();
	return *this;
}

BoundingBox &BoundingBox::translate(const Pos &delta)
{
	mat = TransMat(delta) * mat;
	applyTransform();
	return *this;
}

BoundingBox &BoundingBox::rotate(const ElAg &dea)
{
	mat *= TransMat(dea);
	applyTransform();
	return *this;
}

bool BoundingBox::intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const
{
	t = INF;
	double s;	// intersection
	hit = nullptr;
	for (const Object *obj : objects) {
		if (obj->geo->intersect(ray, s) && s < t) {
			t = s;
			hit = obj;
		}
	}
	if (hit == nullptr) return false;    // no intersection
	x = ray.org + ray.dir * t;
	normal = hit->geo->normalAt(x);
	return true;
}

BoundingBox::~BoundingBox()
{
	for (Object *obj : objects) {
		delete obj;
	}
}

void BoundingBox::fromObjFile(const String &obj_path, double zoom_ratio, const Color &color, const Emission &emi,
							  Object::ReflType reft)
{
	objects = std::move(Parser::parseObjFile(obj_path, zoom_ratio, color, emi, reft));
//	objects = Parser::parseObjFile(obj_path, zoom_ratio, color, emi, reft);
	findBounds();
}
