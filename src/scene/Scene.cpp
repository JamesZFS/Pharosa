//
// Created by James on 2019/4/2.
//

#include "Scene.h"
#include "KDNode.h"
#include "../utils/funcs.hpp"
#include "../geometric/Finite.h"

#include <algorithm>

Scene::Scene() : kd_root(nullptr)
{
}

Scene::~Scene()
{
	for (Object *obj : objects)
		delete obj;
	for (Object *obj : meshes)
		delete obj;
	for (Material *material: materials)
		delete material;
	delete kd_root;    // delete kd tree
}

// !!
bool Scene::intersectAny(const Ray &ray, Intersection &isect) const
{
	real t = INF;    // intersection
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

void Scene::prepare()
{
	// build KD tree
	message("building KD-Tree...");
	ObjectList remove_list;
	for (Object *obj : objects) {
		printf("for %s\n", obj->name.data());
		auto shape = dynamic_cast<Finite *>(obj->geo);
		if (shape) {
			meshes.push_back(obj);
			remove_list.push_back(obj);
		}
	}
	for (Object *obj : remove_list) {
		printf("remove %s\n", obj->name.data());
		auto it = std::find(objects.begin(), objects.end(), obj);
		objects.erase(it);
	}
	delete kd_root;
	kd_root = new KDNode(meshes);
	message("KD-Tree built. max depth = " << __max_depth__);
	debug("  match counter   = %ld\n", __counter__);
	__counter__ = 0;

	// find light sources
	light_sources.clear();
	for (const auto obj : objects) {
		if (obj->mtr->emi.hasPositivePart())
			light_sources.push_back(obj);
	}
	for (const auto obj : meshes) {
		if (obj->mtr->emi.hasPositivePart())
			light_sources.push_back(obj);
	}
	printf("found %ld light sources in total.\n", light_sources.size());
}

size_t Scene::getSingletonCount()
{
	return objects.size();
}

size_t Scene::getMeshCount()
{
	return meshes.size();
}

const List<const Object *> &Scene::getLightSources() const
{
	return light_sources;
}
