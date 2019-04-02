//
// Created by think on 2019/4/1.
//

#include "src/Render.h"
#include <iostream>

using namespace std;

int main()
{
	debug("\n\nHello Ray Tracking!\n");

	/*Actors::Object ob({0, 0, 0}, WHITE);
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
	}*/

	Cameras::BasicCamera cam({0, 100, 0}, {0, 0, 0});
	debug("ex = (%.2f, %.2f, %.2f)\n", cam.ex.x, cam.ex.y, cam.ex.z);
	debug("ey = (%.2f, %.2f, %.2f)\n", cam.ey.x, cam.ey.y, cam.ey.z);
	debug("ez = (%.2f, %.2f, %.2f)\n", cam.ez.x, cam.ez.y, cam.ez.z);
	int cnt = 0;
	while (cam.progress() < 1.0) {
		++cnt;
		auto &r = cam.shootRay();
		if (cnt < 10) {
			debug("r.dir = ");
			r.dir.report();
			debug(", \tr.org = ");
			r.org.report(true);
		}
		assert(r.org.z - Cameras::Camera::CAMERA_FOCUS < EPS);
	}
	cout << cnt << endl;
	assert(cnt == cam.size);

	debug("\n\n");
	return 0;
}
