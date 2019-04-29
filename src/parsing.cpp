//
// Created by James on 2019/4/23.
// implement all parsing

#include "defs.h"
#include <fstream>
#include "scene/Scene.h"

#include "camera/All.h"
#include "alg/All.h"
#include "geometric/All.h"
#include "Renderer.h"
#include "utils/parsers/json.hpp"
#include "utils/parsers/MeshParser.h"

#include "parsing.inl"
#include "alg/Algorithm.h"
#include "scene/Material.h"


Renderer::Renderer(const String &config_path)
{
	Json json;
	std::ifstream fin;
	fin.open(config_path, std::ios::in);

	if (!fin.is_open()) {    // exception
		fin.close();
		TERMINATE("IO Error: config_path \"%s\" cannot be opened, reading stopped.", config_path.data());
	}

	fin >> json;
	fin.close();

	// parsing generic
	String save_path = json.at("save_path");
	if (Funcs::endsWith(save_path, ".ppm")) {
		save_ppm_path = save_path;
		save_cpt_path = save_path.replace(save_path.size() - 4, 4, ".cpt");
	}
	else {
		save_ppm_path = save_path + ".ppm";
		save_cpt_path = save_path + ".cpt";
	}
	String prev_path = json.value("prev_path", "");
	if (prev_path.length() > 0) {
		if (Funcs::endsWith(prev_path, ".ppm")) {
			prev_ppm_path = prev_path;
			prev_cpt_path = prev_path.replace(prev_path.size() - 4, 4, ".cpt");
		}
		else {
			prev_ppm_path = prev_path + ".ppm";
			prev_cpt_path = prev_path + ".cpt";
		}
	}
	else {
		prev_ppm_path = prev_cpt_path = "";
	}

	n_epoch = json.at("n_epoch");
	save_step = json.value("save_step", 0);
	verbose_step = json.value("verbose_step", 0);

	// camera
	camera = Camera::acquire(json.at("camera"));

	// scene
	scene = Scene::acquire(json.at("scene"));

	// algorithm
	algorithm = Algorithm::acquire(json.at("algorithm"), *scene);

	// load checkpoint
	getReady();
}

// camera
Camera *Camera::acquire(const Json &json)
{
	String type = json.value("type", "basic");    // default
	Parsing::lowerStr_(type);
	if (type == "basic") {
		return new BasicCamera(json);
	}
	else if (type == "ortho") {
		return new OrthoCamera(json);
	}
	else if (type == "DOF") {
		//	camera = new DOFCamera(json);
		return nullptr;
	}
	else TERMINATE("Error: got unidentified camera type \"%s\".", type.data());
}


Camera::Camera(const Json &json) :
		Camera(Pos(json.at("pos")), ElAg(json.at("rot")),
			   (size_t) json.value("width", 1024), (size_t) json.value("height", 768), json.value("pixel_size", 0.1))
{
}


BasicCamera::BasicCamera(const Json &json) :
		Camera(json), focus(json.value("focus", 140.0))
{
}

OrthoCamera::OrthoCamera(const Json &json) : Camera(json)
{
}

// alogrithm
Algorithm *Algorithm::acquire(const Json &json, Scene &scene)
{
	String type = json.at("type");
	Parsing::lowerStr_(type);
	if (type == "ray casting" || type == "rc") {
		try {
			return new RayCasting(scene, Dir(json.at("light_dir")));
		}
		catch (Json::out_of_range &) {
			return new RayCasting(scene);    // use default
		}
	}
	else if (type == "ray tracing" || type == "rt") {
		try {
			return new RayTracing(scene, (size_t) json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			return new RayTracing(scene);    // use default
		}
	}
	else TERMINATE("Error: got unidentified algorithm type \"%s\".", type.data());
}

// create scene from json
Scene *Scene::acquire(const Json &json)   // json should be an array
{
	auto *self = new Scene;
	String type;
	Material *material, *sub_material;
	Geometry *geo;
	for (const Json &item: json) {	// for each item in the scene json list
		// shared attributes
		material = Material::acquire(item);			// material
		self->materials.push_back(material);
		TransMat trans_mat(item);					// transform
		// todo texture
		type = item.value("type", "singleton");    	// item type
		Parsing::lowerStr_(type);

		if (type == "singleton") {	// switch different types

			geo = Geometry::acquire(item.at("geo")); // new geo
			geo->applyTransform(trans_mat);
			self->objects.push_back(new Object(geo, material));

		}
		else if (type == "group") {

			Json group = item.at("objects");
			if (group.is_string()) {	// specify by path
				std::ifstream fin;
				fin.open(group, std::ios::in);
				if (!fin.is_open()) {
					fin.close();
					TERMINATE("Error, the specified group object path cannot be opened.");
				}
				fin >> group;
				fin.close();
			}
			if (!group.is_array()) TERMINATE("Error, got invalid group object type.");
			for (const Json &object: group) {	// parse each object in the group
				if (object.has("color") || object.has("emission") || object.has("reft") || object.has("texture")) {
					sub_material = new Material;//::acquire(object);	// new a customized mtr
					sub_material->color = object.has("color") ? Color(object["color"]) : material->color;
					sub_material->emi = object.has("emission") ? Emission(object["emission"]) : material->emi;
					sub_material->reft = object.has("reft") ? Map::str_to_reft[object["reft"]] : material->reft;
					// todo texture
					self->materials.push_back(sub_material);
				}
				else {
					sub_material = material;
				}
				geo = Geometry::acquire(object.has("geo") ? object["geo"] : object);	// new geo
				if (object.has("tra") || object.has("rot")) {
					geo->applyTransform(trans_mat * TransMat(object));
				}
				else {
					geo->applyTransform(trans_mat);
				}
				self->objects.push_back(new Object(geo, sub_material));
			}

		}
		else if (type == "obj" || type == "obj file") {

			auto obj_objects = Parser::parseMeshes(item.at("path"), item.value("scale", 1.0), trans_mat, material);
			self->meshes.insert(self->objects.end(), obj_objects->begin(), obj_objects->end());

		}
		else TERMINATE("Error, got unidentified scene type \"%s\"", type.data());

	}
	return self;
}

Material *Material::acquire(const Json &json)
{
	auto self = new Material;
	try {
		self->color = Color(json.at("color"));
	}
	catch (Json::out_of_range &) {}
	try {
		self->emi = Emission(json.at("emission"));
	}
	catch (Json::out_of_range &) {}
	try {
		self->reft = Map::str_to_reft.at(json.value("reft", "DIFF"));
	}
	catch (std::out_of_range &) {
		TERMINATE("Error, got invalid mtr type \"%s\".", json["mtr"].get<String>().data());
	}
	return self;
}

// geometries
Geometry *Geometry::acquire(const Json &json)
{
	String type = json.at("type");
	Parsing::lowerStr_(type);
	if (type == "cube") {
		return Cube::acquire(json);
	}
	else if (type == "infplane") {
		return InfPlane::acquire(json);
	}
	else if (type == "sphere") {
		return Sphere::acquire(json);
	}
	else if (type == "triangle") {
		return Triangle::acquire(json);
	}
	else TERMINATE("Error, got invalid geometry type \"%s\".", type.data());
}

InfPlane *InfPlane::acquire(const Json &json)
{
	return new InfPlane(Dir(json.at("normal")), Pos(json.at("point")));
}

Cube *Cube::acquire(const Json &json)
{
	if (json.has("basis")) {
		return new Cube(Pos(json.at("basis").at(0)), Pos(json.at("basis").at(1)), Pos(json.at("basis").at(2)),
						json.has("pos") ? Pos(json["pos"]) : Pos());
	}
	else {
		return new Cube(json.value("length", 1.0), json.value("width", 1.0), json.value("height", 1.0),
						json.has("pos") ? Pos(json["pos"]) : Pos());
	}
}

Sphere *Sphere::acquire(const Json &json)
{
	return new Sphere(json.value("radius", 1.0), json.has("pos") ? Pos(json["pos"]) : Pos());
}

Triangle *Triangle::acquire(const Json &json)
{
	return new Triangle(json.at("points").at(0), json.at("points").at(1), json.at("points").at(2));
}

// core: using constructor's name for expediency
Pos::Pos(const Json &json) : Pos(json.at(0), json.at(1), json.at(2))    // construct from json
{
}

RGB::RGB(const Json &json) : r(x), g(y), b(z)
{
	if (json.is_array()) {
		x = json.at(0);
		y = json.at(1);
		z = json.at(2);
	}
	else if (json.is_string()) {    // default colors
		try {
			*this = *Map::str_to_rgb.at(json);
		}
		catch (std::out_of_range &) TERMINATE("Error, got unidentified RGB \"%s\".", json.get<String>().data());
	}
	else TERMINATE("Error, got invalid RGB type.");
}

ElAg::ElAg(const Json &json) :    // use degrees
		ElAg(json.at(0).get<double>() * DEG, json.at(1).get<double>() * DEG, json.at(2).get<double>() * DEG)
{
}

// transform matrix
TransMat::TransMat(const Json &json) :    // should have "tra" and "rot" key, if not - identical transform
		TransMat(json.has("tra") ? Pos(json["tra"]) : Pos(), json.has("rot") ? ElAg(json["rot"]) : ElAg())
{
}
