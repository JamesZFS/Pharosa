//
// Created by James on 2019/4/19.
//

#include "Parsers.h"

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
	size_t a, b, c;
	List<Pos> v;

	while (!fin.eof()) {
		fin >> mark;
		switch (mark) {
			case '#':
				fin.getline(buffer, 200);    // comment
				break;

			case 'v':    // vertex (x, y, z)
				fin >> x >> y >> z;
				v.emplace_back(x * zoom_ratio, y * zoom_ratio, z * zoom_ratio);
				break;

			case 'f':    // face (rank1, rank2, rank3), notice this rank starts from 1
				fin >> a >> b >> c;
				--a, --b, --c;
				meshes.push_back(new Object(Triangle({{v[a], v[b], v[c]}}, Pos()), color, emi, reft));
				break;

			default:
				sprintf(buffer, "Error: got unidentified mark \"%c\", parsing stopped.", mark);
				warn(buffer);
				fin.close();
				exit(1);
		}
	}
	return std::move(meshes);
//	return meshes;
}
