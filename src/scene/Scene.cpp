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
bool Scene::intersectAny(const Ray &ray, const Object *&hit, Pos &x, Dir &normal) const
{
	double t = INF, ti;    // intersection
	hit = nullptr;
	// for singletons
	for (const Object *obj : objects) {
		if (obj->geo->intersect(ray, ti)) {
			if (ti < t) {    // update first intersection
				t = ti;
				hit = obj;
				x = ray.org + ray.dir * t;
			}
		}
	}
	if (hit) normal = hit->geo->normalAt(x);
	// for KD-Tree
	assert(kd_root != nullptr);
	return hit || kd_root->intersectAny(ray, t, hit, x, normal);
}

const Object *Scene::hitOf(const Ray &ray) const    // return hit without normal and x
{
	const Object *hit = nullptr;
	Pos x;
	Dir normal;
	intersectAny(ray, hit, x, normal);
	return hit;
}

void Scene::buildKDTree()
{
	delete kd_root;
	message("building KD-Tree...");
	kd_root = new KDNode(meshes);
	message("KD-Tree built.");
}

size_t Scene::getSingletonCount()
{
	return objects.size();
}

size_t Scene::getMeshCount()
{
	return meshes.size();
}
