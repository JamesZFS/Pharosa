//
// Created by James on 2019/4/23.
// implement all parsing

#include "utils/parsers/json.hpp"
#include <fstream>

#include "scene/Scene.h"
#include "camera/All.h"
#include "alg/All.h"
#include "Renderer.h"

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

	// parsing json
	String type;

	save_path = json.at("save_path");
	checkpoint_dir = json.value("checkpoint_dir", "");    // get with default value
	prev_path = json.value("prev_path", "");
	n_epoch = json.at("n_epoch");
	prev_epoch = json.value("prev_epoch", 0);
	verbose_step = json.value("verbose_step", 0);
	Funcs::generator.seed((unsigned int) time(nullptr));

	// camera
	Json cam_json = json.at("camera");
	type = cam_json.value("type", "basic");
	if (type == "basic") {
		camera = new BasicCamera(cam_json);
	}
	else if (type == "ortho") {
		camera = new OrthoCamera(cam_json);
	}
	else if (type == "DOF") {
		//	camera = new DOFCamera(cam_json);
	}
	else TERMINATE("Error: got unidentified camera type \"%s\".", type.data());
	// load checkpoint
	if (prev_path.length() > 0) {
		camera->readPPM(prev_path, prev_epoch);
	}

//	message(cam_json.dump(3));

	// scene
	Json scene_json = json.at("scene");
	scene = new Scene(scene_json);

//	message(scene_json.dump(3));

	// algorithm
	Json alg_json = json.at("algorithm");
	type = alg_json.at("type");
	if (type == "RayCasting" || type == "ray casting" || type == "RC") {
		try {
			algorithm = new RayCasting(*scene, *camera, Dir(alg_json.at("light_dir")));
		}
		catch (Json::out_of_range &) {
			algorithm = new RayCasting(*scene, *camera);	// use default
		}
	}
	else if (type == "RayTracing" || type == "ray tracing" || type == "RT") {
		try {
			algorithm = new RayTracing(*scene, *camera, (size_t) alg_json.at("max_depth"));
		}
		catch (Json::out_of_range &) {
			algorithm = new RayTracing(*scene, *camera);	// use default
		}
	}
	else TERMINATE("Error: got unidentified algorithm type \"%s\".", type.data());

	algorithm->info();
//	message(alg_json.dump(3));
}

// camera
Camera::Camera(const Json &json) :
		Camera(Pos(json.at("pos")), ElAg(json.at("rot")),
			   (size_t) json.value("width", 1024), (size_t) json.value("height", 768), json.value("pixel_size", 0.1))
{
}


BasicCamera::BasicCamera(const Json &json) :
		Camera(json), focus(json.value("focus", 140.0))
{

}


// scene
Scene::Scene(const Json &json)
{
	String type;
	Object *object;
	Geometry *geo;
	for (const Json &item: json) {
		type = item.value("type", "singleton");

		// switch todo
		if (type == "singleton") {

		}
		else if (type == "group") {

		}
		else if (type == "obj" || type == "obj file") {

		}
		else TERMINATE("Error, got unidentified scene type \"%s\"", type.data());
	}
}


// vector
Pos::Pos(const Json &json) : Pos(json.at(0), json.at(1), json.at(2))	// construct from json
{
}

RGB::RGB(const Json &json) : RGB(json.at(0), json.at(1), json.at(2))
{
}

ElAg::ElAg(const Json &json) :    // use degrees
		ElAg(json.at(0).get<double>() * DEG, json.at(1).get<double>() * DEG, json.at(2).get<double>() * DEG)
{
}