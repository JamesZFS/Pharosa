//
// Created by James on 2019/4/19.
//

#include "Parsers.h"

#define FAIL { sprintf(buffer, "Error: got unidentified mark \"%s\", parsing stopped.", mark.data()); warn(buffer); fin.close(); exit(1); }
#define SKIP_LINE { fin.getline(buffer, 200); break; }

ObjectList Parser::parseObjFile(const String &obj_path, double zoom_ratio, const Color &color,
								const Emission &emi, Object::ReflType reft)    // load mesh segments from objects file
{
	std::ifstream fin;
	fin.open(obj_path, std::ios::in);

	Buffer buffer;
	if (!fin.is_open()) {
		sprintf(buffer, "Error: obj_path \"%s\" cannot be opened, parsing stopped.", obj_path.data());
		warn(buffer);
		fin.close();
		exit(1);
	}

	ObjectList meshes;    // result
	String mark, temp;
	char flag;
	double x, y, z;
	size_t ord[3];
	List<Pos> v;
#ifdef __DEV_STAGE__
	double x_min = INF, x_max = -INF, y_min = INF, y_max = -INF, z_min = INF, z_max = -INF;
#endif

	while (!fin.eof()) {
		fin >> mark;
		flag = mark[0];
		switch (flag) {
			case 'v':    // vertex (x, y, z)
				if (mark.length() >= 2) FAIL
				fin >> x >> y >> z;

#ifdef __DEV_STAGE__
				x_min = min2(x_min, x);
				x_max = max2(x_max, x);

				y_min = min2(y_min, y);
				y_max = max2(y_max, y);

				z_min = min2(z_min, z);
				z_max = max2(z_max, z);
#endif

				v.emplace_back(x * zoom_ratio, y * zoom_ratio, z * zoom_ratio);
				SKIP_LINE

			case 'f':    // face (rank1, rank2, rank3), notice this rank starts from 1
				for (size_t &i : ord) {
					fin >> temp;
					if (temp.find("//") == String::npos) {
						i = std::stoul(temp);
					}
					else {    // like "9290//15997"
						char *ptr;
						i = std::strtoul(temp.data(), &ptr, 10);
					}
				}
				meshes.push_back(new Object(Triangle({{v[ord[0]], v[ord[1]], v[ord[2]]}}, Pos()), color, emi, reft));
				SKIP_LINE

			case 'l':
			case 'm':
			case 'o':
			case 's':
			case 'u':
			case '#':
				SKIP_LINE

			default: FAIL    // unidentified
		}
	}
#ifdef __DEV_STAGE__
	warn("Parser::parseObjFile(), Bounds found:");
	warn(" xmin = " << x_min << "  xmax = " << x_max);
	warn(" ymin = " << y_min << "  ymax = " << y_max);
	warn(" zmin = " << z_min << "  zmax = " << z_max);
	warn("\n");
	warn("vn count = " << __counter__ << "\n");
	fflush(stdout);
	fflush(stderr);
#endif

	return std::move(meshes);
//	return meshes;
}

#undef FAIL
