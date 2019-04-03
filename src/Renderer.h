//
// Created by think on 2019/4/1.
//

#ifndef RAYTRACKER_RENDERER_H
#define RAYTRACKER_RENDERER_H

#include "lib.h"
#include "Vec.hpp"
#include "Ray.hpp"
#include "scene/Stage.h"
#include "camera/All.h"
#include "alg/All.h"

// frontend of all, the Renderer
template <typename Algorithm, typename CameraType>
class Renderer
{
private:
	Algorithm illuminator;
	CameraType camera;
	Stage stage;
public:
	Renderer() = default;
};

#include "Renderer.cpp"

#endif //RAYTRACKER_RENDERER_H
