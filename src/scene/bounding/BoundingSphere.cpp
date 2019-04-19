//
// Created by James on 2019/4/19.
//

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(Sphere &&sphere_, ObjectList &objects_) :
		BoundingBox(objects_, sphere_.mat), sphere(sphere_)
{
}

void BoundingSphere::applyTransform()
{
	BoundingBox::applyTransform();
#ifdef __DEV_STAGE__
	warn("BoundingSphere::applyTransform()\n");
#endif
	sphere.mat = mat;
	sphere.applyTransform();
}

bool BoundingSphere::intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const
{
	return sphere.intersect(ray, t) && BoundingBox::intersectAny(ray, t, hit, x, normal);
}
