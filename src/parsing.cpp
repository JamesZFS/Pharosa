//
// Created by James on 2019/4/23.
// implement all parsing methods here

#include "Pharosa.h"
#include "scene/Scene.h"

#include "camera/All.h"
#include "alg/All.h"
#include "geometric/All.h"
#include "Renderer.h"
#include "utils/parsers/json.hpp"
#include "utils/parsers/MeshParser.h"
#include "alg/Algorithm.h"
#include "scene/Material.hpp"

#include "parsing.inl"
#include "geometric/Geometry.h"
#include "geometric/PolyRev.h"

#include <fstream>


Renderer::Renderer(const String &config_path) : Renderer()
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
	setup(json);
}

void Renderer::setup(const Json &json)
{
	clear();
	printf("loading...\n");
	// parsing generic
	save_path = json.at("save_path");
	if (!Funcs::endsWith(save_path, ".ppm")) {
		save_path += ".ppm";
	}
	prev_path = json.value("prev_path", "");
	if (prev_path.length() > 0 && !Funcs::endsWith(prev_path, ".ppm")) {
		prev_path += ".ppm";
	}

	n_epoch = json.at("n_epoch");
	save_step = json.value("save_step", 0);
	verbose_step = json.value("verbose_step", 0);

	// camera
	camera = Camera::acquire(json.at("camera"));

	// scene
	scene = Scene::acquire(json.at("scene"));

	// algorithm
	algorithm = Algorithm::acquire(json.at("algorithm"), *scene, *camera);

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
	else if (type == "dof") {
		return new DOFCamera(json);
	}
	else TERMINATE("Error: got unidentified camera type \"%s\".", type.data());
}


Camera::Camera(const Json &json) :
		Camera(Pos(json.at("pos")), ElAg(json.at("rot")),
			   (size_t) json.value("width", 1024), (size_t) json.value("height", 768), json.value("pixel_size", 0.1f))
{
}


BasicCamera::BasicCamera(const Json &json) :
		Camera(json), length(json.value("length", 140.0f)),
		ez_length(ez * length), pos_ez_length(pos + ez_length)
{
}

OrthoCamera::OrthoCamera(const Json &json) : Camera(json)
{
}


DOFCamera::DOFCamera(const Json &json) :
		Camera(json),
		length(json.value("length", 20.0f)), focus(json.value("focus", 10.0f)), aperture(json.value("aperture", 1.0f)),
		f_l(focus / length), pos_ez_length(pos + ez * length), ez_focus(ez * focus)
{
}

// algorithm
Algorithm *Algorithm::acquire(const Json &json, const Scene &scene, Camera &camera)
{
	String type = json.at("type");
	Parsing::lowerStr_(type);
	if (type == "ray casting" || type == "rc") {
		try {
			return new RayCasting(scene, camera, Dir(json.at("light_dir")));
		}
		catch (Json::out_of_range &) {
			return new RayCasting(scene, camera);    // use default
		}
	}
	else if (type == "path tracing" || type == "pt") {
		try {
			return new PathTracing(scene, camera, (size_t) json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			return new PathTracing(scene, camera);    // use default
		}
	}
	else if (type == "path tracing explicit" || type == "pte") {
		try {
			return new PTE(scene, camera, (size_t) json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			return new PTE(scene, camera);    // use default
		}
	}
	else if (type == "path tracing forward" || type == "ptf") {
		try {
			return new PTF(scene, camera, (size_t) json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			return new PTF(scene, camera);    // use default
		}
	}
	else TERMINATE("Error: got unidentified algorithm type \"%s\".", type.data());
}

// create scene from json
Scene *Scene::acquire(const Json &json)   // json should be an array
{
	auto *self = new Scene;
	String type, name;
	Material *material, *sub_material;
	Geometry *geo;
	for (const Json &item: json) {    // for each item in the scene json list
		// shared attributes
		material = Material::acquire(item);            // material
		self->materials.push_back(material);
		TransMat trans_mat(item);                    // transform
		type = item.value("type", "singleton");        // item type
		Parsing::lowerStr_(type);
		name = item.value("name", "");

		if (type == "singleton") {    // switch different types

			geo = Geometry::acquire(item.at("geo")); // new geo
			geo->applyTransform(trans_mat);
			self->objects.push_back(new Object(geo, material, name));

		}
		else if (type == "group") {

			Json group = item.at("objects");
			if (group.is_string()) {    // specify by path
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
			for (const Json &object: group) {    // parse each object in the group
				if (object.has("color") || object.has("emission") || object.has("reft") || object.has("texture")) {
					sub_material = Material::acquire(object);    // new a customized mtr
					self->materials.push_back(sub_material);
				}
				else {
					sub_material = material;
				}
				geo = Geometry::acquire(object.has("geo") ? object["geo"] : object);    // new geo
				if (object.has("tra") || object.has("rot")) {
					geo->applyTransform(trans_mat * TransMat(object));
				}
				else {
					geo->applyTransform(trans_mat);
				}
				String name_ = object.value("name", name);
				self->objects.push_back(new Object(geo, sub_material, name_));
			}

		}
		else if (type == "obj" || type == "obj file") {

			auto obj_objects = Parser::parseMeshes(item.at("path"), item.value("scale", 1.0f), trans_mat, material);
			self->meshes.insert(self->meshes.end(), obj_objects->begin(), obj_objects->end());

		}
		else if (type == "skip") continue;
		else TERMINATE("Error, got unidentified scene type \"%s\"", type.data());

	}
	return self;
}

Material *Material::acquire(const Json &json)
{
	auto self = new Material;	// default material
	if (json.has("color")) self->color = Color(json["color"]);
	if (json.has("emission")) self->emi = Emission(json["emission"]);
	if (json.has("diff")) self->diff = json["diff"];
	if (json.has("spec")) self->spec = json["spec"];
	if (json.has("refr")) self->refr = json["refr"];
	if (json.has("n_refr")) self->refr = json["n_refr"];
	if (json.has("texture")) {
		const Json &t_json = json["texture"];
		if (t_json.is_string()) {
			self->texture = new Image(String(t_json));
		}
		else {
			self->texture = new Image(String(t_json.at("path")));
			if (t_json.has("scale")) {
				real  scale = t_json["scale"];
				self->Auu = scale;
				self->Avv = scale;
			}
			if (t_json.has("mat")) {
				const Json &mat = t_json["mat"];
				self->Auu = mat.at(0).at(0);
				self->Auv = mat.at(0).at(1);
				self->Auc = mat.at(0).at(2);
				self->Avu = mat.at(1).at(0);
				self->Avv = mat.at(1).at(1);
				self->Avc = mat.at(1).at(2);
			}
		}
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
	else if (type == "poly revolution" || type == "poly rev") {
		return PolyRev::acquire(json);
	}
	else if (type == "bezier revolution" || type == "bezier rev") {
		return BezierRev::acquire(json);
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
		return new Cube(json.value("length", 1.0f), json.value("width", 1.0f), json.value("height", 1.0f),
						json.has("pos") ? Pos(json["pos"]) : Pos());
	}
}

Sphere *Sphere::acquire(const Json &json)
{
	return new Sphere(json.value("radius", 1.0f), json.has("pos") ? Pos(json["pos"]) : Pos());
}

Triangle *Triangle::acquire(const Json &json)
{
	return new Triangle(json.at("points").at(0), json.at("points").at(1), json.at("points").at(2));
}

PolyRev *PolyRev::acquire(const Json &json)
{
	List<real > &&x_coeffs = json.at("x_coeffs"), &&y_coeffs = json.at("y_coeffs");
	return new PolyRev(x_coeffs, y_coeffs);
}

BezierRev *BezierRev::acquire(const Json &json)
{
	return new BezierRev(json.at("control_points"));
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
		ElAg(json.at(0).get<real >() * DEG, json.at(1).get<real >() * DEG, json.at(2).get<real >() * DEG)
{
}

// transform matrix
TransMat::TransMat(const Json &json) :    // should have "tra" and "rot" key, if not - identical transform
		TransMat(json.has("tra") ? Pos(json["tra"]) : Pos(), json.has("rot") ? ElAg(json["rot"]) : ElAg())
{
}
