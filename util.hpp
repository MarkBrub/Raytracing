#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions
inline constexpr double degrees_to_radians(const double degrees) {
	return degrees * pi / 180.0;
}

inline constexpr double clamp(const double x, const double min, const double max) {
	if(x < min) return min;
	if(x > max) return max;
	return x;
}

//Random
inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(const double min, const double max) {
	return min + ((max - min) * random_double());
}

//Includes
#include "vec3.hpp"
#include "ray.hpp"