//
// Created by think on 2019/4/1.
//

#include "src/Render.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	Actors::Object ob({0, 0, 0}, WHITE);
	Actors::Sphere sphere({0, 0, 0}, 2, WHITE);
	Pos q[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
	Actors::Triangle tr({0, 0, 0}, q, WHITE);
	for (auto &p : tr.gp) {
		cout << p.x << ' ' << p.y << ' ' << p.z << endl;
	}
	cout << endl;
	tr.rotate({M_PI_2, M_PI_4, 0});
	for (auto &p : tr.gp) {
		cout << p.x << ' ' << p.y << ' ' << p.z << endl;
	}

	Cameras::Camera cam({0, 0, 0}, {1, 1, 1});

	for (unsigned int i = 0; i < 500; ++i) {
		for (unsigned int j = 0; j < 50; ++j) {
			cam.renderInc(i, j, {1, 0.1, 0});
			cam.renderInc(j, i, {0, 0, 1});
		}
	}
	cam.writePPM("../out/test3");

	debug("\n\n");
	return 0;
}
