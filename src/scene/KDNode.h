//
// Created by James on 2019/4/28.
//

#ifndef PHAROSA_KDNODE_H
#define PHAROSA_KDNODE_H

#include "../Pharosa.h"
#include "BoundingBox.h"
#include "Object.h"

// unit of KD-Tree
class KDNode
{
private:
	BoundingBox *box;
	ObjectList *tris;
	KDNode *l_child;
	KDNode *r_child;

	void build(const ObjectList &triangles, size_t depth);

public:
	KDNode();    // empty node

	KDNode(const ObjectList &triangles);    // build KD-node

	~KDNode();

	void intersect(const Ray &ray, double &t, Intersection &isect) const;	// update first intersection
};

// typedef List<Object *> ObjectList;

#endif //PHAROSA_KDNODE_H
