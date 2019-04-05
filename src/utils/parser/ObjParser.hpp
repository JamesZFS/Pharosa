//
// Created by James on 2019/4/5.
//

#ifndef PHAROSA_OBJPARSER_HPP
#define PHAROSA_OBJPARSER_HPP

#include "../../lib.h"
#include "../funcs.hpp"
#include "../../scene/Triangle.h"
#include "../../scene/MeshObj.h"

TriangleGroup fromObjFile(const String &obj_path)	// load mesh segments from objects file
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

	TriangleGroup meshes;	// result
	char mark;
	double x, y, z;
	unsigned int a, b, c;
	Pos p[3];
	List<Pos> v;

	while (!fin.eof()) {
		fin >> mark;
		switch (mark) {
			case '#':
				fin.getline(buffer, 200);	// comment
				break;

			case 'v':	// vertex (x, y, z)
				fin >> x >> y >> z;
				v.emplace_back(x, y, z);
				break;

			case 'f':	// face (rank1, rank2, rank3)
				fin >> a >> b >> c;
				p[0] = v[a];
				p[1] = v[b];
				p[2] = v[c];
				// todo multicolor, emission, texture support
				meshes.push_back(new Triangle(Pos::ORIGIN, p, Color::WHITE));
				break;

			default:
				sprintf(buffer, "Error: got unidentified mark \"%c\", parsing stopped.", mark);
				warn(buffer);
				fin.close();
				exit(1);
		}
	}
	return std::move(meshes);
}

#endif //PHAROSA_OBJPARSER_HPP
