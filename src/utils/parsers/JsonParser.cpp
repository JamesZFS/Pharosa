////
//// Created by James on 2019/4/3.
////
//
//#include "JsonParser.h"
//#include "../funcs.hpp"
//#include <nlohmann/json.hpp>
//// todo use makefile to compile .o first to exclude this file
//
//// ** init static member variables **
//Pos JsonParser::pos;
//Color JsonParser::color;
//Emission JsonParser::emission;
//ElAg JsonParser::euler_angles;
//Object::ReflType JsonParser::refl_type;
//double JsonParser::radius;
//Pos JsonParser::points[3];
//String JsonParser::obj_str;
//const Object *JsonParser::result;
//
//ObjectList JsonParser::fromJsonFile(const String &config_path)
//{
//	using json = nlohmann::json;
//	std::ifstream fin;
//	fin.open(config_path, std::ios::in);
//	Buffer buffer;
//
//	if (!fin.is_open()) {    // exception
//		sprintf(buffer, "IO Error: config_path \"%s\" cannot be opened, reading stopped.", config_path.data());
//		warn(buffer);
//		fin.close();
//		exit(1);
//	}
//
//	json config;
//	fin >> config;
//	fin.close();
//	ObjectList objects;
//
//	// for each Object defined in config
//	for (auto &item : config.items()) {
//		json j_obj = item.value();
//		String type = j_obj["type"].get<String>();
//		obj_str = j_obj.dump();
//		Funcs::toLower(type);    // into lower cases
//
//		// switch type
//		if (type == "sphere") {
//			JsonParser::parseSphere();
//		}
//		else if (type == "triangle") {
//			JsonParser::parseTriangle();
//		}
//		else if (type == "object" || type == "obj") {
//			JsonParser::parseObj();
//		}
//		else {
//			sprintf(buffer, "Error: got unidentified Object type \"%s\", parsing stops.\n", type.data());
//			warn(buffer);
//			exit(1);
//		}
//		objects.push_back(result);
//	}
//	return objects;
//}
//
//void JsonParser::reset()
//{
//	// assign default values
//	emission = Emission::DARK;
//	euler_angles = ElAg::NONROT;
//	refl_type = Object::DIFF;
//}
//
//void JsonParser::parseGeneric()
//{
//	reset();
//	using json = nlohmann::json;
//	json j_obj = json::parse(obj_str), j_tmp;
//	String s_tmp;
//
//	// required attributes:
//	if (j_obj.find("pos") == j_obj.end()) {
//		warn("Error: attribute \"pos\" not found, parsing stops.\n");
//		exit(1);
//	}
//	j_tmp = j_obj["pos"];
//	pos = {j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>()};
//
//	if (j_obj.find("color") == j_obj.end()) {
//		warn("Error: attribute \"color\" not found, parsing stops.\n");
//		exit(1);
//	}
//	j_tmp = j_obj["color"];
//	color = {j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>()};
//
//	// optional attributes:
//	if (j_obj.find("emission") != j_obj.end()) {
//		j_tmp = j_obj["emission"];
//		emission = {j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>()};
//	}
//	if (j_obj.find("angles") != j_obj.end()) {
//		j_tmp = j_obj["angles"];	// notice that this is degree angle
//		euler_angles = ElAg(j_tmp[0].get<double>() * M_PI / 180.0,
//							j_tmp[1].get<double>() * M_PI / 180.0,
//							j_tmp[2].get<double>() * M_PI / 180.0);
//	}
//	if (j_obj.find("reflection") != j_obj.end()) {
//		j_tmp = j_obj["reflection"];
//		s_tmp = j_tmp.get<String>();
//		Funcs::toLower(s_tmp);
//		if (s_tmp == "diffusive") {
//			refl_type = Object::DIFF;
//		}
//		else if (s_tmp == "reflective") {
//			refl_type = Object::REFL;
//		}
//		else if (s_tmp == "refractive") {
//			refl_type = Object::REFR;
//		}
//		else {
//			warn("Error: got unidentified \"reflection\" attribute, parsing stops.\n");
//			exit(1);
//		}
//	}
//}
//
//void JsonParser::parseSphere()
//{
//	parseGeneric();
//
//	using json = nlohmann::json;
//	json j_obj = json::parse(obj_str);
//
//	if (j_obj.find("radius") == j_obj.end()) {
//		warn("Error: attribute \"radius\" not found, parsing stops.\n");
//		exit(1);
//	}
//	radius = double(j_obj["radius"].get<double>());
//	result = new Sphere(pos, radius, color, emission, euler_angles, refl_type);
//}
//
//void JsonParser::parseTriangle()
//{
//	parseGeneric();
//
//	using json = nlohmann::json;
//	json j_obj = json::parse(obj_str), j_tmp;
//
//	if (j_obj.find("points") == j_obj.end()) {
//		warn("Error: attribute \"points\" not found, parsing stops.\n");
//		exit(1);
//	}
//	for (int i = 0; i < 3; ++i) {
//		j_tmp = j_obj["points"][i];
//		points[i] = Pos(j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>());
//	}
//	result = new Triangle(pos, points, color, emission, euler_angles, refl_type);
//}
//
//void JsonParser::parseObj()
//{
//	parseGeneric();
//
//	using json = nlohmann::json;
//	json j_obj = json::parse(obj_str), j_tmp;
//
//	if (j_obj.find("path") == j_obj.end()) {
//		warn("Error: attribute \"path\" not found, parsing stops.\n");
//		exit(1);
//	}
//
//	// todo implement from obj
//}
