//
// Created by James on 2019/4/19.
//

#include "BoundingCube.h"

BoundingCube::BoundingCube(Cube &&cube_, ObjectList &objects_) :
		BoundingBox(objects_, cube_.mat), cube(cube_)
{
}

void BoundingCube::applyTransform()
{
	BoundingBox::applyTransform();
#ifdef __DEV_STAGE__
	warn("BoundingCube::applyTransform()\n");
#endif
	cube.mat = mat;
	cube.applyTransform();
}

bool BoundingCube::intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const
{
	return cube.intersect(ray, t) && BoundingBox::intersectAny(ray, t, hit, x, normal);
}
