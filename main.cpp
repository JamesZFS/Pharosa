//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
//#include "src/utils/json.hpp"
#include <iostream>

using namespace std;
using json = nlohmann::json;

int main()
{
	Parser::reset();
	debug("\n\nHello Ray Tracking!\n");

//	json config;
//	ifstream fin("../config/1.json");
//	fin >> config;
//	fin.close();
//
//	for (auto &el : config.items()) {
//		cout << el.key() << " : " << el.value() << "\n";
//		json j_obj = el.value();
//		cout << j_obj["type"].get<String>() << endl;
//		if (j_obj.find("color") != j_obj.end()) {
//			auto &j_tmp = j_obj["color"];
//			Color color = {j_tmp[0].get<double>(), j_tmp[1].get<double>(), j_tmp[2].get<double>()};
//			color.report(true);
//		}
//		cout << endl;
//	}

	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
	renderer.setupStage("../config/1.json");
	renderer.setupCamera(Pos::ORIGIN, ElAg::NONROT);
	renderer.start(5);
	renderer.save("../out/test.ppm");

	debug("\n\n");
	return 0;
}
