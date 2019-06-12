//
// Created by James on 2019/4/2.
//

#ifndef PHAROSA_SCENE_H
#define PHAROSA_SCENE_H

#include "../Pharosa.h"
#include "../core/Ray.hpp"
#include "Intersection.h"
#include "Object.h"

class KDNode;

// Scene class, for i/o models and calculating L1 intersection
class Scene
{
private:
	ObjectList objects; // all singleton object ptrs, using pointer to allow polymorphism
	ObjectList meshes;	// store triangles parsed from mesh file
	List<Material *> materials;    // mtr is shared while geometry belongs to objects
	KDNode *kd_root;	// root of KD-tree
	List<const Object *> light_sources;	// all objects with positive emission

public:
	Scene();        	// init an empty scene

	~Scene();

	void prepare();		// build KD-Tree from kd_root for meshes, and find all light sources

	size_t getSingletonCount();    // count objects in total

	size_t getMeshCount();    // count BoundingBox in total

	const List<const Object *> &getLightSources() const;  // get all objects with positive emission and finite size

	/** judge and calculate first intersection with all objects
	 * input:
	 * @param ray
	 * @param comp_hit_only
	 *
	 * output:
	 * @param isect : intersection info
	 *
	 * @return true if intersects any object else false
	 */
	bool intersectAny(const Ray &ray, Intersection &isect, bool comp_hit_only = false) const;

	static Scene *acquire(const Json &json);    // load from json
};

#endif //PHAROSA_SCENE_H
