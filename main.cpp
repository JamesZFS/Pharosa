//
// Created by think on 2019/4/1.
//

#include "src/Renderer.h"
#include "src/utils/json.hpp"
#include <iostream>

using namespace std;
using json = nlohmann::json;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	ifstream fin("config/example.json");
	json config;
//	cin >> config;
	fin >> config;
	fin.close();

	for (auto &el : config.items()) {
		cout << el.key() << " : " << el.value() << "\n";
		json obj = el.value();
		cout << obj["type"].get<String>() << endl;
		if (obj.find("color") != obj.end()) {
			cout << obj["color"].get<String>() << endl;
		}
		cout << endl;
	}

//	cout << config.dump(4);

//	Renderer<Algorithms::RayTracing, Cameras::BasicCamera> renderer;
//	renderer.setupCamera(Pos::ORIGIN, ElAg::NONROT);
//	renderer.start(1);
//	renderer.save("../out/hhh");

	debug("\n\n");
	return 0;
}
