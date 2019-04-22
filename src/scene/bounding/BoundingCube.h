//
// Created by James on 2019/4/19.
//

#ifndef PHAROSA_BOUNDINGCUBE_H
#define PHAROSA_BOUNDINGCUBE_H


#include "../BoundingBox.h"

class BoundingCube : public BoundingBox
{
private:
	Cube cube;
#ifdef __DEV_STAGE__
	Object *ocube;	// todo only for debugging
#endif

public:
	BoundingCube();

	BoundingCube(ObjectList &objects_);

	BoundingCube(Cube &&cube_, ObjectList &objects_);	// manual

	void applyTransform() override;

	bool intersectAny(const Ray &ray, double &t, const Object *&hit, Pos &x, Dir &normal) const override;

	void findBounds() override;
};


#endif //PHAROSA_BOUNDINGCUBE_H
