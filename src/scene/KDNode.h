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
	ObjectList *objs;
	KDNode *l_child;
	KDNode *r_child;

	void build(const ObjectList &finite_objs, size_t depth);

public:
	KDNode();    // empty node

	KDNode(const ObjectList &finite_objs);    // build KD-node
	
	~KDNode();

	void intersect(const Ray &ray, real &t, Intersection &isect) const;	// update first intersection
};

// typedef List<Object *> ObjectList;

#endif //PHAROSA_KDNODE_H
