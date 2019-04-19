//
// Created by James on 2019/4/19.
//

#ifndef PHAROSA_BOUNDINGCUBE_H
#define PHAROSA_BOUNDINGCUBE_H


#include "BoundingBox.h"

class BoundingCube : public BoundingBox
{
private:
	Cube cube;

public:
	BoundingCube() = default;

	BoundingCube(Cube &&cube_, ObjectList &objects_);

	void applyTransform() override;

	bool intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const override;

	void findBounds() override;
};


#endif //PHAROSA_BOUNDINGCUBE_H
