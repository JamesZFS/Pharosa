//
// Created by James on 2019/4/19.
//

#include "MeshParser.h"
#include "../../geometric/Triangle.h"
#include <fstream>


#define FAIL { \
sprintf(buffer, "Error: got unidentified mark \"%c\", parsing stopped.", mark); \
warn(buffer); \
fin.close(); \
exit(1); }
#define SKIP_LINE { fin.getline(buffer, 200); break; }

ObjectList *Parser::parseMeshes(const String &obj_path, real scale, const TransMat &trans_mat,
								const Material *material)// load mesh segments from objects file
{
	printf("parsing meshes from %s", obj_path.data());
	fflush(stdout);
	std::ifstream fin;
	fin.open(obj_path, std::ios::in);

	if (!fin.is_open()) {
		fin.close();
		TERMINATE("Error: obj_path \"%s\" cannot be opened, parsing stopped.", obj_path.data());
	}

	auto *meshes = new ObjectList;    // result
	Triangle *mesh;
	char mark;
	Buffer buffer;
	real x, y, z;
#ifdef __DEV_STAGE__
	real x_min = INF, x_max = -INF, y_min = INF, y_max = -INF, z_min = INF, z_max = -INF;
#endif
	size_t a, b, c;
	List<Pos> v;

	while (!fin.eof()) {
		mark = '.';
		fin >> mark;
		if (mark == '.') continue; // no input
		switch (mark) {
			case '#':    // comment
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
				v.emplace_back(x * scale, y * scale, z * scale);    // new point with scaling
				SKIP_LINE

			case 'f':    // face (rank1, rank2, rank3), notice this rank starts from 1
				fin >> a >> b >> c;
				mesh = new Triangle(v.at(--a), v.at(--b), v.at(--c));    // new a mesh
				mesh->applyTransform(trans_mat);                         // transform
				meshes->push_back(new Object(mesh, material));            // save
				SKIP_LINE

			default: FAIL
		}
	}
#ifdef __DEV_STAGE__
	warn("Parser::parseMeshes(), Bounds found:");
	warn(" xmin = " << x_min << "  xmax = " << x_max);
	warn(" ymin = " << y_min << "  ymax = " << y_max);
	warn(" zmin = " << z_min << "  zmax = " << z_max);
	warn("\n");
#endif

	return meshes;
}

#undef FAIL
#undef SKIP_LINE
