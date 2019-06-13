//
// Created by James on 2019/4/2.
//

#include "Scene.h"
#include "KDNode.h"
#include "../utils/funcs.hpp"
#include "../geometric/Finite.h"
#include "../geometric/Cube.h"
#include "../geometric/Triangle.h"

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
bool Scene::intersectAny(const Ray &ray, Intersection &isect, bool comp_hit_only) const
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
#if KD_TREE_ON
	kd_root->intersect(ray, t, isect);
	if (!isect.hit) return false;
#else
	for (const Object *obj : meshes) {
		if (obj->geo->intersect(ray, t, isect)) {
			isect.hit = obj;
		}
	}
#endif

	// if intersected:
	if (comp_hit_only) return true;
	isect.pos = ray.posAfter(t);
	isect.r_in = ray;
	isect.complementData();
	return true;
}

void Scene::prepare()
{
	ObjectList remove_list;
	// convert cubes into triangles
#if CONVERT_CUBE_TO_MESHES_ON
	for (Object *obj : objects) {
		auto cube = dynamic_cast<Cube *>(obj->geo);    // if it's a finite, move it to meshes
		if (!cube) continue;
		auto &c = *cube;
		remove_list.push_back(obj);
		List<Triangle *> tris =
				{
						new Triangle(c.o + c.oy, c.o + c.ox, c.o),
						new Triangle(c.o + c.oy, c.o + c.ox + c.oy, c.o + c.ox),
						new Triangle(c.o + c.oz, c.o + c.oy, c.o),
						new Triangle(c.o + c.oz, c.o + c.oy + c.oz, c.o + c.oy),
						new Triangle(c.o + c.ox, c.o + c.oz, c.o),
						new Triangle(c.o + c.ox, c.o + c.oz + c.ox, c.o + c.oz),
						new Triangle(c.o + c.oz + c.ox, c.o + c.oz + c.oy, c.o + c.oz),
						new Triangle(c.o + c.oz + c.oy, c.o + c.oz + c.ox, c.o + c.oz + c.ox + c.oy),
						new Triangle(c.o + c.ox + c.oy, c.o + c.ox + c.oz, c.o + c.ox),
						new Triangle(c.o + c.ox + c.oz, c.o + c.ox + c.oy, c.o + c.ox + c.oy + c.oz),
						new Triangle(c.o + c.oy + c.oz, c.o + c.oy + c.ox, c.o + c.oy),
						new Triangle(c.o + c.oy + c.ox, c.o + c.oy + c.oz, c.o + c.oy + c.oz + c.ox),
				};
		// get 12 triangles
		for (auto tri : tris) {
			auto mesh= new Object(tri, obj->mtr, obj->name);
			meshes.push_back(mesh);
		}
	}
#endif
#if REPORT_SINGLETON
	printf("\033[34m\n---Singletons Info:---\n");
	for (Object *obj : objects) {
		obj->report();
	}
	printf("\n----------------------\n\033[0m");
#endif
	// move to mesh
	for (Object *obj : objects) {
		auto shape = dynamic_cast<Finite *>(obj->geo);    // if it's a finite, move it to meshes
		if (shape) {
			meshes.push_back(obj);
			remove_list.push_back(obj);
		}
	}
	// dump trash
	for (Object *obj : remove_list) {
		auto it = std::find(objects.begin(), objects.end(), obj);
		objects.erase(it);
	}
#if KD_TREE_ON
	// build KD tree
	message("building KD-Tree...");
	delete kd_root;
	kd_root = new KDNode(meshes);
	debug("KD-Tree built. max depth = %ld\n", __kdnode_max_depth__);
	debug("  match counter   = %ld\n", __counter__);
	__counter__ = 0;
#endif

	// find light sources
	light_sources.clear();
	for (const auto obj : objects) {
		auto shape = dynamic_cast<Finite *>(obj->geo);
		if (!shape) continue;	// deal only with finite light sources
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
