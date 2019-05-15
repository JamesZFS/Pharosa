//
// Created by James on 2019/4/2.
//

#include "Scene.h"
#include "KDNode.h"
#include "../utils/funcs.hpp"


Scene::Scene() : kd_root(nullptr)
{
}

Scene::~Scene()
{
	for (Object *obj : objects) {
		delete obj;
	}
	for (Material *material: materials) {
		delete material;
	}
	delete kd_root;
}

// !!
bool Scene::intersectAny(const Ray &ray, Intersection &isect) const
{
	double t = INF;    // intersection
	isect.hit = nullptr;
	// for singletons
	for (const Object *obj : objects) {
		if (obj->geo->intersect(ray, t, isect)) {
			isect.hit = obj;
		}
	}
	// for KD-Tree
//	assert(kd_root != nullptr);
	kd_root->intersect(ray, t, isect);
	if (!isect.hit) return false;

	// intersected:
	isect.pos = ray.travels(t);
	isect.complementData();
	return true;
}

void Scene::buildKDTree()
{
	delete kd_root;
	message("building KD-Tree...");
	kd_root = new KDNode(meshes);
	message("KD-Tree built. max depth = " << __max_depth__);
	debug("  match counter   = %ld\n", __counter__);
}

size_t Scene::getSingletonCount()
{
	return objects.size();
}

size_t Scene::getMeshCount()
{
	return meshes.size();
}
