#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <array>
#include <functional>

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "util.hpp"

#include "improved_bvh.hpp"


class bvh_node : public hittable {
public:
    bvh_node() = default;

    bvh_node(hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end);

    bvh_node(const improved_bvh_node& node) {
        left = node.left;
        right = node.right;
        box = node.box;
    }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(aabb& output_box) const override;

public:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

bvh_node create_bvh_tree(const hittable_list& list);