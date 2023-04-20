#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <array>
#include <functional>
#include <numeric>

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "util.hpp"


class improved_bvh_node : public hittable {
public:
    improved_bvh_node() = default;

    improved_bvh_node(hittable_list& list) : improved_bvh_node(list.objects, 0, list.objects.size()) {}

    improved_bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end);

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(aabb& output_box) const override;

private:
    double split_sah(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end, size_t split) const;
    double surface_area(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end) const;

public:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};