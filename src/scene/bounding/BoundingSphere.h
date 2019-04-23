//
// Created by James on 2019/4/19.
//

#ifndef PHAROSA_BOUNDINGSPHERE_H
#define PHAROSA_BOUNDINGSPHERE_H

#include "BoundingBox.h"

class BoundingSphere : public BoundingBox
{
private:
	Sphere sphere;

public:
	BoundingSphere(Sphere &&sphere_, ObjectList &objects_);

	void applyTransform() override;

	bool intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const override;

	void findBounds() override;
};


#endif //PHAROSA_BOUNDINGSPHERE_H
