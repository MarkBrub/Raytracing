#pragma once

#include "aabb.hpp"
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
	virtual bool bounding_box(aabb& output_box) const = 0;
};

class translate : public hittable {
public:
	translate(std::shared_ptr<hittable> p, const vec3& displacement)
		: ptr(p), offset(displacement) {}

	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	std::shared_ptr<hittable> ptr;
	vec3 offset;
};

class rotate_y : public hittable {
public:
	rotate_y(std::shared_ptr<hittable> p, const double angle);

	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override {
		output_box = bbox;
		return hasbox;
	}

public:
std::shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
};;

bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis);
bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);