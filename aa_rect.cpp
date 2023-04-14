#include "aa_rect.hpp"

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// Check that the ray in in a valid range for a hit.
	auto t = (k - r.origin.z()) / r.direction.z();
	if (t < t_min || t_max < t) {
		return false;
	}
	
	// Check that the ray hit the rectangle.
	auto x = r.origin.x() + t * r.direction.x();
	auto y = r.origin.y() + t * r.direction.y();
	if (x < x0 || x > x1 || y < y0 || y > y1) {
		return false;
	}

	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.p = r.at(t);
	rec.set_face_normal(r, vec3(0, 0, 1));

	return true;
}

bool xy_rect::bounding_box(aabb& output_box) const {
	// The bounding box must have non-zero width in each dimension, so pad the Z
	// dimension a small amount.
	output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
	return true;
}

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// Check that the ray in in a valid range for a hit.
	auto t = (k - r.origin.y()) / r.direction.y();
	if (t < t_min || t_max < t) {
		return false;
	}
	
	// Check that the ray hit the rectangle.
	auto x = r.origin.x() + t * r.direction.x();
	auto z = r.origin.z() + t * r.direction.z();
	if (x < x0 || x > x1 || z < z0 || z > z1) {
		return false;
	}
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.p = r.at(t);
	rec.set_face_normal(r, vec3(0, 1, 0));
	return true;
}

bool xz_rect::bounding_box(aabb& output_box) const {
	// The bounding box must have non-zero width in each dimension, so pad the Y
	// dimension a small amount.
	output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
	return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// Check that the ray in in a valid range for a hit.
	auto t = (k - r.origin.x()) / r.direction.x();
	if (t < t_min || t_max < t) {
		return false;
	}
	
	// Check that the ray hit the rectangle.
	auto y = r.origin.y() + t * r.direction.y();
	auto z = r.origin.z() + t * r.direction.z();
	if (y < y0 || y > y1 || z < z0 || z > z1) {
		return false;
	}
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.p = r.at(t);
	rec.set_face_normal(r, vec3(1, 0, 0));
	return true;
}

bool yz_rect::bounding_box(aabb& output_box) const {
	// The bounding box must have non-zero width in each dimension, so pad the X
	// dimension a small amount.
	output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
	return true;
}