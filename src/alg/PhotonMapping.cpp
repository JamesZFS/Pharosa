//
// Created by James on 2019/6/7.
//

#include "PhotonMapping.h"

PhotonMapping::PhotonMapping(const Scene &scene_) : Algorithm(scene_)
{

}

PhotonMapping::~PhotonMapping()
{

}

String PhotonMapping::info() const
{
	return "<Photon Mapping>";
}

Color PhotonMapping::radiance(const Ray &ray) const
{

}
