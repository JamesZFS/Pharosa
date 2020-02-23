//
// Created by James on 2020/2/23.
//

#ifndef PHAROSA_SHADOWMAP_H
#define PHAROSA_SHADOWMAP_H

#include "Algorithm.h"

class ShadowMap : public Algorithm {
public:
    ShadowMap(const Scene &scene, Camera &camera);

    String info() const override;

    void start(size_t n_epoch, size_t save_step, const std::function<void(size_t)> &pre_epoch_callback,
               const std::function<void(size_t)> &in_epoch_callback,
               const std::function<void(size_t)> &post_epoch_callback) override;

private:
    List2D<real> Wlv_buffer, Wl_buffer;

    void estimateShadow(const Intersection &surface, real &Wlv, real &Wl);

    void capture(); // refresh camera film
};


#endif //PHAROSA_SHADOWMAP_H
