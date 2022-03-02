#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <iostream>
#include <iomanip>

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
	return rand() / (RAND_MAX + 1.0);

	//this is a better random but is much slower
	//static std::uniform_real_distribution<double> distribution(0, 1);
	//static std::mt19937 generator;
	//return distribution(generator);
}

inline double random_double(const double min, const double max) {
	return min + ((max - min) * random_double());
}

inline void print_progress_bar(const double progress, const double maxValue) {
	std::ios::sync_with_stdio(false);
	const int BAR_LENGTH = 80;
	std::string bar(BAR_LENGTH, ' ');

	double fill_amount = progress * BAR_LENGTH / maxValue;

	for(int x = 0; x <= fill_amount; x++) {
		bar[x] = static_cast<char>(0xDB);
	}

	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	std::cout << "\rProgress: [" << bar << "] " << progress * 100 / maxValue << "%     " << std::flush;
}