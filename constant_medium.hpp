#pragma once

#include "util.hpp"
#include "hittable.hpp"
#include "material.hpp"

class constant_medium : public hittable {
public:
    constant_medium(std::shared_ptr<hittable> b, double d, color c)
        : boundary(b), neg_inv_density(-1 / d), phase_function(std::make_shared<isotropic>(c)) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override {
        return boundary->bounding_box(output_box);
    }


public:
    std::shared_ptr<hittable> boundary;
    std::shared_ptr<material> phase_function;
    double neg_inv_density;
};