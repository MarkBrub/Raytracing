#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

struct sphere : public hittable {
	point3 center;
	double radius;
	std::shared_ptr<material> mat_ptr;

	sphere() = default;
	sphere(const point3& c, const double r, std::shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {}

	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;
};