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
	// translate objects to 0 - length | width | height
	for (auto *obj : objects) {
		obj->geo->translate(delta);
	}
	applyTransform();
}
