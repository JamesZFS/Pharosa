//
// Created by think on 2019/4/1.
//

#include "Renderer.h"

Renderer::Renderer(const Algorithms::GI &algorithm_, const Cameras::Camera &camera_) :
		illuminator(algorithm_), camera(camera_)
{
}

void Renderer::setStage(const String &config_path)
{
	stage.from_config(config_path);
}
