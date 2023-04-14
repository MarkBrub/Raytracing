#pragma once

#include "util.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class camera {
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 w;
	vec3 u;
	vec3 v;
	double lens_radius = 0;

public:
	camera() = default;
	camera(
		const point3& lookfrom, const point3& lookat, const point3& vup, const double vfov,
		const double aspect_ratio, const double aperture, const double focus_dist
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;
	}

	ray get_ray(const double s, const double t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(origin + offset, lower_left_corner + (s * horizontal) + (t * vertical) - origin - offset);
	}
};