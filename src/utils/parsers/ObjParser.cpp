//
// Created by James on 2019/4/19.
//

#include "ObjParser.h"

#define FAIL { sprintf(buffer, "Error: got unidentified mark \"%c\", parsing stopped.", mark); warn(buffer); fin.close(); exit(1); }
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
	char mark;
	double x, y, z;
#ifdef __DEV_STAGE__
	double x_min = INF, x_max = -INF, y_min = INF, y_max = -INF, z_min = INF, z_max = -INF;
#endif
	size_t a, b, c;
	List<Pos> v;

	while (!fin.eof()) {
		fin >> mark;
		switch (mark) {
			case '#':	// comment
				SKIP_LINE

			case 'v':    // vertex (x, y, z)
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
				fin >> a >> b >> c;
				--a, --b, --c;
				meshes.push_back(new Object(Triangle({{v[a], v[b], v[c]}}, Pos()), color, emi, reft));
				SKIP_LINE

			default:
				FAIL
		}
	}
#ifdef __DEV_STAGE__
	warn("Parser::parseObjFile(), Bounds found:");
	warn(" xmin = " << x_min << "  xmax = " << x_max);
	warn(" ymin = " << y_min << "  ymax = " << y_max);
	warn(" zmin = " << z_min << "  zmax = " << z_max);
	warn("\n");
#endif

	return std::move(meshes);
//	return meshes;
}

#undef FAIL
#undef SKIP_LINE
