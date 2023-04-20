#include "improved_bvh.hpp"

improved_bvh_node::improved_bvh_node(
    std::vector<std::shared_ptr<hittable>>& objects,
    size_t start, size_t end) {

    const std::array<bool (*)(const std::shared_ptr<hittable>, const std::shared_ptr<hittable>), 3> comparators = {
        box_x_compare, box_y_compare, box_z_compare
    };
   

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        // These objects are already sorted by the axis, so we can just assign them
        left = objects[start];
        right = objects[start + 1];
    }
    else {
        // Test all three axes and splits to find the best SAH
        double best_sah = std::numeric_limits<double>::infinity();
        int best_axis = 0;
        int best_split = 0;

        for (int axis = 0; axis < 3; axis++) {
            // Sort the objects by the current axis
            std::sort(objects.begin() + start, objects.begin() + end, comparators[axis]);

            // Test all possible splits
            for (size_t split = start + 1; split < end; split++) {
                // Get the sah for this split
                double sah = split_sah(objects, start, end, split);

                if (sah < best_sah) {
					best_sah = sah;
					best_axis = axis;
					best_split = split;
				}
			}
        }


        std::sort(objects.begin() + start, objects.begin() + end, comparators[best_axis]);
        // auto mid = start + best_split;

        left = std::make_shared<improved_bvh_node>(objects, start, best_split);
        right = std::make_shared<improved_bvh_node>(objects, best_split, end);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(box_left) || !right->bounding_box(box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    box = surrounding_box(box_left, box_right);
}


bool improved_bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max)) {
        return false;
    }

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool improved_bvh_node::bounding_box(aabb& output_box) const {
    output_box = box;
    return true;
}

double improved_bvh_node::split_sah(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end, size_t split) const {
    // Get left and right side and their surface areas
    double left_surface_area = surface_area(objects, start, split);
    double right_surface_area = surface_area(objects, split, end);

    double total_surface_area = surface_area(objects, start, end);

    // Calculate probabilities
    double left_probability = left_surface_area / total_surface_area;
    double right_probability = right_surface_area / total_surface_area;

    // Get the number of objects on each side
    size_t left_count = split - start;
    size_t right_count = end - split;

    // Return their combined cost
    return left_probability * left_count + right_probability * right_count;
}

double improved_bvh_node::surface_area(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end) const {
    // If the range is not valid throw output an error
    if (start >= end) {
        std::cerr << "Invalid range in combined_bounding_box.\n";
        return 0;
    }

    // Get a bounding box for the first element
    aabb combined;
    objects[start]->bounding_box(combined);

	// If the first element is the only one, return its surface area
    if (end - start == 1) {
        return combined.surface_area();
    }

    // A lambda for combining boxes
    static auto combine_boxes = [](const aabb& box1, const std::shared_ptr<hittable>& obj) {
        aabb box2;
        obj->bounding_box(box2);

        return surrounding_box(box1, box2);
    };

    // Accumulate a combined aabb
    combined = std::accumulate(objects.begin() + start, objects.begin() + end, combined, combine_boxes);

    // Return it's surface area
    return combined.surface_area();
}