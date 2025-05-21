#include "acceleration/bvh.hpp"

bvh_node::bvh_node(
    std::vector<std::shared_ptr<hittable>>& objects,
    size_t start, size_t end) {

    int axis = random_int(0, 2);
    std::array<std::function<bool(const std::shared_ptr<hittable>&, const std::shared_ptr<hittable>&)>, 3> comparator_array = { box_x_compare, box_y_compare, box_z_compare };
    auto comparator = comparator_array[axis];

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        left = objects[start];
        right = objects[start + 1];
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<bvh_node>(objects, start, mid);
        right = std::make_shared<bvh_node>(objects, mid, end);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(box_left) || !right->bounding_box(box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    box = surrounding_box(box_left, box_right);
}


bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max)) {
        return false;
    }

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool bvh_node::bounding_box(aabb& output_box) const {
    output_box = box;
    return true;
}

bvh_node create_bvh_tree(const hittable_list& list) {
    std::vector<std::shared_ptr<hittable>> objects = list.objects;
    bvh_node root(objects, 0, objects.size());
    return root;
}