#pragma once

#include <cmath>
#include <array>
#include <iostream>

#include "utils/util.hpp"

struct vec3 {
	std::array<double, 3> e;

	vec3() : e{0, 0, 0} {}
	vec3(const double e1, const double e2, const double e3) : e{e1, e2, e3} {}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(const double min, const double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double& operator[](unsigned int index) { return e[index]; }
	double operator[](unsigned int index) const { return e[index]; }

	vec3& operator+=(const vec3& v) {
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] += t;
		e[1] += t;
		e[2] += t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return std::sqrt(length_squared());
	}

	double length_squared() const {
		return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const double t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(const vec3& v, const double t) {
	return t * v;
}

inline vec3 operator/(const vec3& v, const double t) {
	return v * (1 / t);
}

double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(const vec3& v);
vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_in_hemisphere(const vec3& normal);
vec3 random_in_unit_disk();
vec3 reflect(const vec3& v, const vec3& n);
vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat);

// Type aliases for vec3
using point3 = vec3;
using color = vec3;