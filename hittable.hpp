#pragma once

#include "vec3.hpp"
#include "ray.hpp"

struct material;

struct hit_record {
	point3 p;
	vec3 normal;
	std::shared_ptr<material> mat_ptr;
	double t = 0;
	bool front_face = true;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

struct hittable {
	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const = 0;
};