//
// Created by James on 2019/4/19.
//

#include "BoundingCube.h"

BoundingCube::BoundingCube() : cube(), ocube(nullptr)
{
}

BoundingCube::BoundingCube(ObjectList &objects_) : BoundingBox(objects_)
{
	findBounds();
}

//BoundingCube::BoundingCube(Cube &&cube_, ObjectList &objects_) :
//		BoundingBox(objects_, cube_.mat), cube(cube_)
//{
//}

void BoundingCube::applyTransform()
{
//	BoundingBox::applyTransform();
//	cube.mat = mat;
//	cube.applyTransform();
//#ifdef __DEV_STAGE__
//	delete ocube;
//	ocube = new Object(Cube(cube),
//					   Color::WHITE, Emission(0.5, 0, 0), Object::REFR);
//#endif
}

bool BoundingCube::intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const
{
	// todo
//	if (cube.intersect(ray, t)) {
//		hit = ocube;
//		x = ray.org + ray.dir * t;
//		normal = hit->geo->normalAt(x);
//		return true;
//	}
//	return false;
	return cube.intersect(ray, t) && BoundingBox::intersectAny(ray, t, hit, x, normal);
}

void BoundingCube::findBounds()
{
	double x_min = INF, x_max = -INF, y_min = INF, y_max = -INF, z_min = INF, z_max = -INF;
	// find bounds
	for (const auto *obj : objects) {
		x_min = min2(x_min, obj->geo->xMin());
		x_max = max2(x_max, obj->geo->xMax());

		y_min = min2(y_min, obj->geo->yMin());
		y_max = max2(y_max, obj->geo->yMax());

		z_min = min2(z_min, obj->geo->zMin());
		z_max = max2(z_max, obj->geo->zMax());
	}

#ifdef __DEV_STAGE__
	warn("Bounds found:");
	warn(" xmin = " << x_min << "  xmax = " << x_max);
	warn(" ymin = " << y_min << "  ymax = " << y_max);
	warn(" zmin = " << z_min << "  zmax = " << z_max);
	warn("\n");
#endif

	cube = Cube(x_max - x_min, y_max - y_min, z_max - z_min);
	const Pos delta(-x_min, -y_min, -z_min);
//	mat = TransMat(delta);
	// translate objects to 0 - length | width | height
//	for (auto *obj : objects) {
//		obj->geo->translateLocal(delta);
//	}
	debug("\n");
	applyTransform(); // todo local crd problem

#ifdef __DEV_STAGE__

	x_min = INF, x_max = -INF, y_min = INF, y_max = -INF, z_min = INF, z_max = -INF;
	for (const auto *obj : objects) {
		x_min = min2(x_min, obj->geo->xMin());
		x_max = max2(x_max, obj->geo->xMax());

		y_min = min2(y_min, obj->geo->yMin());
		y_max = max2(y_max, obj->geo->yMax());

		z_min = min2(z_min, obj->geo->zMin());
		z_max = max2(z_max, obj->geo->zMax());
	}
	warn("after translation:");
	warn(" xmin = " << x_min << "  xmax = " << x_max);
	warn(" ymin = " << y_min << "  ymax = " << y_max);
	warn(" zmin = " << z_min << "  zmax = " << z_max);
	warn("\n");
#endif
}
