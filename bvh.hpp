#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "util.hpp"


class bvh_node : public hittable {
public:
    bvh_node() = default;

    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(
        const std::vector<std::shared_ptr<hittable>>& src_objects,
        size_t start, size_t end);

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(aabb& output_box) const override;

public:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis);
bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);

bvh_node create_bvh_tree(const hittable_list& list);