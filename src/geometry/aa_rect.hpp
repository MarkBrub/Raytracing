#pragma once

#include "utils/util.hpp"
#include "scene/hittable.hpp"

class xy_rect : public hittable {
public:
	xy_rect() = default;
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	double x0, x1, y0, y1, k;
	std::shared_ptr<material> mat_ptr;
};

class xz_rect : public hittable {
public:
	xz_rect() = default;
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<material> mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	double x0, x1, z0, z1, k;
	std::shared_ptr<material> mat_ptr;
};

class yz_rect : public hittable {
public:
	yz_rect() = default;
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<material> mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	double y0, y1, z0, z1, k;
	std::shared_ptr<material> mat_ptr;
};