#pragma once

#include "vec3.hpp"

struct ray {
	point3 origin;
	vec3 direction;

	ray() = default;
	ray(const point3& o, const vec3& d) : origin(o), direction(d) {}

	point3 at(const double t) const {
		return origin + (t * direction);
	}
};