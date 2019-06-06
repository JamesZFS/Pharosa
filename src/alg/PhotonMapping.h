//
// Created by James on 2019/6/7.
//

#ifndef PHAROSA_PHOTONMAPPING_H
#define PHAROSA_PHOTONMAPPING_H


#include "Algorithm.h"

class PhotonMapping : public Algorithm
{
private:

public:
	PhotonMapping(const Scene &scene_);

	~PhotonMapping() override;

	String info() const override;

	Color radiance(const Ray &ray) const override;
};


#endif //PHAROSA_PHOTONMAPPING_H
