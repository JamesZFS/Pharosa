//
// Created by James on 2019/6/7.
//

#ifndef PHAROSA_PHOTONMAPPING_H
#define PHAROSA_PHOTONMAPPING_H


#include "Algorithm.h"

class PhotonMapping : public Algorithm
{
protected:
	const size_t n_iteration;
	const size_t n_photon_per_iter;
	const size_t max_depth;

public:
	PhotonMapping(const Scene &scene_, Camera &camera_, size_t n_iteration_, size_t n_photon_per_iter_, size_t max_depth_);

	~PhotonMapping() override;

	String info() const override;

	Color radiance(const Ray &ray) const override;
};


#endif //PHAROSA_PHOTONMAPPING_H
