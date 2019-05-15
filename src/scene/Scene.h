//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_SCENE_H
#define PHAROSA_SCENE_H

#include "../Pharosa.h"
#include "../core/Ray.hpp"
#include "Object.h"

class KDNode;

// Scene class, for i/o models and calculating L1 intersection
class Scene
{
private:
	ObjectList objects;    // all singleton object ptrs, using pointer to allow polymorphism
	List<Material *> materials;    // mtr is shared while geometry belongs to objects
	ObjectList meshes;	// store triangles parsed from mesh file
	KDNode *kd_root;	// root of KD-tree

public:
	Scene();        	// init an empty scene

	~Scene();

	void buildKDTree();		// build KD-Tree from kd_root for meshes

	size_t getSingletonCount();    // count objects in total

	size_t getMeshCount();    // count BoundingBox in total

	/** judge and calculate first intersection with all objects
	 * input:
	 * @param ray
	 *
	 * output:
	 * @param hit : pointer of object first hit
	 * @param x : hitting point
	 * @param normal : normal vector of hitting point
	 *
	 * @return true if intersects any object else false
	 */
	bool intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const;

	const Object *hitOf(const Ray &ray) const;    // compute only the first hit target of L1

	static Scene *acquire(const Json &json);    // load from json
};

#endif //PHAROSA_SCENE_H
