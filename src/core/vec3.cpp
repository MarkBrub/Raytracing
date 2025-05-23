#include "core/vec3.hpp"

double dot(const vec3& u, const vec3& v) {
	return (u[0] * v[0]) + (u[1] * v[1]) + (u[2] * v[2]);
}

vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}

vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

vec3 random_in_unit_sphere() {
	while(true) {
		auto p = vec3::random(-1, 1);
		if(p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if(dot(in_unit_sphere, normal) > 0) {
		return in_unit_sphere;
	}
	return -in_unit_sphere;
}

vec3 random_in_unit_disk() {
	while(true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if(p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - ((2 * dot(v, n)) * n);
}

vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + (cos_theta * n));
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}