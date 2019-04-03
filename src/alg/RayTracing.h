//
// Created by James on 2019/4/2.
//

#ifndef RAYTRACKER_RAYTRACING_H
#define RAYTRACKER_RAYTRACING_H

#include "GI.h"

// Ray Tracing algorithm
class RayTracing : public GI
{
private:
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;

	double randf();	// random float [0.0, 1.0]

public:
	RayTracing(const Stage &stage_, Cameras::BasicCamera &camera_, unsigned int random_seed);

	void render(unsigned int n_epoch) override;

protected:
	Color radiance(const Ray &ray, unsigned int depth);	// main alg
};

#include "RayTracing.cpp"

#endif //RAYTRACKER_RAYTRACING_H
