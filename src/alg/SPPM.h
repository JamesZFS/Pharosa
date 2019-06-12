//
// Created by James on 2019/6/7.
//

#ifndef PHAROSA_SPPM_H
#define PHAROSA_SPPM_H

#include "Algorithm.h"
#include "PMGrid.h"
#include "VisiblePoint.h"

// Stochastic Progressive Photon Mapping
class SPPM : public Algorithm
{
	// pass 1:
	// let camera shoot rays
	// for each shot ray, trace its path in the scene to form visible points
	// build kd tree to hold all visible points
	//
	// pass 2:
	// for each photon in n_photon_per_iteration
	// randomly sample one light from source
	// trace it in the scene and query kd tree for path's nearby visible points
	// contribute photon radiance to kd grid
	//
	// pass over:
	// compute pixel radiance values according to kd node info
protected:
	const size_t n_photon_per_iter;
	const size_t max_depth;
	const real init_radius;
	List2D<VisiblePoint> visible_points;    // owned by this class, camera.width x camera.height
	UniformGrid *grids;
	const List<const Object *> &lights;

	void buildGrid();

	void traceCameraRay(Ray ro, VisiblePoint &vp);

	Ray sampleOneLight(Color &beta);

	void tracePhoton(Ray ri, Color beta, real r_bound);

	void drawPhi(size_t epoch) const;

	void drawLi(size_t epoch) const;

	void drawM(size_t epoch) const;

	void capture(size_t n_epoch);	// flash to camera

public:
	SPPM(const Scene &scene_, Camera &camera_,
		 size_t n_photon_per_iter_ = 1000, size_t max_depth_ = 5, real init_radius_ = 1.f);

	~SPPM() override;

	String info() const override;

	void start(size_t n_epoch, size_t save_step,
			   const std::function<void(size_t)> &pre_epoch_callback,
			   const std::function<void(size_t)> &in_epoch_callback,
			   const std::function<void(size_t)> &post_epoch_callback) override;
};


#endif //PHAROSA_SPPM_H
