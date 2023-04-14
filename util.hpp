#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <iostream>
#include <iomanip>
#include <format>
#include <ranges>

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
	// return rand() / (RAND_MAX + 1.0);

	// this is a better random but is much slower
	static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(const double min, const double max) {
	return min + ((max - min) * random_double());
}

inline int random_int(int min, int max) {
	// Returns a random integer in [min,max].
	return static_cast<int>(random_double(min, max + 1));
}

inline std::string ms_to_time(const uint64_t& ms) {
	std::string time;
	uint64_t seconds = ms / 1000;
	uint64_t minutes = seconds / 60;
	uint64_t hours = minutes / 60;
	seconds %= 60;
	minutes %= 60;
	if (hours > 0) {
		time += std::to_string(hours) + "h ";
	}
	if (minutes > 0) {
		time += std::to_string(minutes) + "m ";
	}
	time += std::to_string(seconds) + "s";
	return time;
}

inline double calculate_eta(const double progress, const double maxValue, const uint64_t& elapsed_time, double& smoothedProgressRate) {
	double alpha = 0.9; // Smoothing factor, experiment with different values (0 < alpha <= 1) to adjust how much recent progress is weighted

	// Calculate the current progress rate
	double currentProgressRate = progress / elapsed_time;

	// Update the smoothed progress rate using the exponentially weighted moving average
	smoothedProgressRate = alpha * currentProgressRate + (1.0 - alpha) * smoothedProgressRate;

	// Calculate the ETA based on the smoothed progress rate
	double remainingProgress = maxValue - progress;
	double eta = remainingProgress / smoothedProgressRate;

	return eta;
}

inline void print_progress_bar(double progress, double maxValue, uint64_t elapsed_time) {
	const int BAR_LENGTH = 80;
	std::string bar(BAR_LENGTH, ' ');

	static int update_count = 0;
	static std::string eta_string = ms_to_time(0);
	double fill_amount = progress * BAR_LENGTH / maxValue;

	// Fill the bar with the progress
	std::ranges::fill(bar.begin(), bar.begin() + static_cast<int>(fill_amount), static_cast<char>(0xDB));

	// Calculate the ETA
	double eta = 0.0;
	eta = (elapsed_time / progress) * (maxValue - progress);

	if (update_count % 10 != 0) {
		eta_string = ms_to_time(eta);
	}

	update_count++;

	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	std::cout << "\rProgress: [" << bar << "] " << progress * 100 / maxValue << "% " << "ETA: " << eta_string << "     " << std::flush;
}