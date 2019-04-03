//
// Created by James on 2019/4/3.
//

#include "Parser.h"
#include "json.hpp"
#include "funcs.hpp"
// todo use makefile to compile .o first to exclude this file

List<const Scenes::Object *> Parser::fromJsonFile(const String &config_path)
{
	using json = nlohmann::json;
	std::ifstream fin;
	fin.open(config_path, std::ios::in);
	char buffer[250];

	if (!fin.is_open()) {    // exception
		sprintf(buffer, "IO Error: config_path \"%s\" cannot be opened, reading stopped.", config_path.data());
		warn(buffer);
		exit(1);
	}

	json config;
	fin >> config;
	fin.close();
	List<const Scenes::Object *> objects;

	// for each Object defined in config
	for (auto &item : config.items()) {
		json j_obj = item.value();
		String type = j_obj["type"].get<String>();
		obj_str = j_obj.get<String>();
		Funcs::toLower(type);    // into lower cases
		reset();    // init results

		// switch type
		if (type == "sphere") {
			Parser::parseSphere();
		}
		else if (type == "triangle") {
			Parser::parseTriangle();
		}
		else if (type == "object" || type == "obj") {
			Parser::parseObj();
		}
		else {
			sprintf(buffer, "Warning: got unidentified Object type \"%s\", continue.\n", type.data());
			warn(buffer);
		}
		objects.push_back(result);
	}
	return objects;
}

#define release(x) delete x; x = nullptr
#define releaseArr(x) delete[] x; x = nullptr

void Parser::reset()
{
	release(pos);
	release(color);
	release(emission);
	release(euler_angles);
	refl_type = Scenes::Object::NONE;

	release(radius);
	releaseArr(points);
}

#undef release
#undef releaseArr

void Parser::parseGeneric()
{
	using json = nlohmann::json;
	json j_obj = json::parse(obj_str), j_tmp;
	String s_tmp;

	// required attributes:
	if (j_obj.find("pos") == j_obj.end()) {
		warn("Error: attribute \"pos\" not found, parsing stops.\n");
		exit(1);
	}
	j_tmp = j_obj["pos"];
	pos = new Pos(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());

	if (j_obj.find("color") == j_obj.end()) {
		warn("Error: attribute \"color\" not found, parsing stops.\n");
		exit(1);
	}
	j_tmp = j_obj["color"];
	color = new Color(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());

	// optional attributes:
	if (j_obj.find("emission") != j_obj.end()) {
		j_tmp = j_obj["emission"];
		emission = new Emission(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());
	}
	if (j_obj.find("angles") != j_obj.end()) {
		j_tmp = j_obj["angles"];
		euler_angles = new ElAg(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());
	}
	if (j_obj.find("reflection") != j_obj.end()) {
		j_tmp = j_obj["reflection"];
		s_tmp = j_tmp.get<String>();
		Funcs::toLower(s_tmp);
		if (s_tmp == "diffusive") {
			refl_type = Scenes::Object::DIFF;
		}
		else if (s_tmp == "reflective") {
			refl_type = Scenes::Object::REFL;
		}
		else if (s_tmp == "refractive") {
			refl_type = Scenes::Object::REFR;
		}
		else {
			warn("Error: got unidentified \"reflection\" attribute, parsing stops.\n");
			exit(1);
		}
	}
}

void Parser::parseSphere()
{
	parseGeneric();

	using json = nlohmann::json;
	json j_obj = json::parse(obj_str);

	if (j_obj.find("radius") == j_obj.end()) {
		warn("Error: attribute \"radius\" not found, parsing stops.\n");
		exit(1);
	}
	radius = new double(j_obj["radius"].get<double>());
}

void Parser::parseTriangle()
{
	parseGeneric();

	using json = nlohmann::json;
	json j_obj = json::parse(obj_str), j_tmp;

	if (j_obj.find("points") == j_obj.end()) {
		warn("Error: attribute \"points\" not found, parsing stops.\n");
		exit(1);
	}
	points = new Pos[3];
	for (int i = 0; i < 3; ++i) {
		j_tmp = j_obj["points"][i];
		points[i] = Pos(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());
	}
}

void Parser::parseObj()
{
	parseGeneric();

	using json = nlohmann::json;
	json j_obj = json::parse(obj_str), j_tmp;

	if (j_obj.find("path") == j_obj.end()) {
		warn("Error: attribute \"path\" not found, parsing stops.\n");
		exit(1);
	}

	// todo implement from obj
}
