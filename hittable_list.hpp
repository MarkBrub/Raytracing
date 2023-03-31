#pragma once

#include <vector>
#include <memory>

#include "hittable.hpp"

struct hittable_list : hittable {
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() = default;
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;
};