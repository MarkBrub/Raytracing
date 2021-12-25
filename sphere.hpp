#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

struct sphere : public hittable {
	point3 center;
	double radius;

	sphere() = default;
	sphere(const point3& c, const double r) : center(c), radius(r) {}

	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const {
	vec3 oc = r.origin - center;
	auto a = r.direction.length_squared();
	auto half_b = dot(oc, r.direction);
	auto c = oc.length_squared() - (radius * radius);

	auto discriminant = (half_b * half_b) - (a * c);
	if(discriminant < 0) return false;
	auto sqrtd = std::sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if(root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if(root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}