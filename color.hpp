#pragma once
#include "vec3.hpp"

void write_color(std::ostream& out, const color& pixel_color, const int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);

	out << static_cast<int>(256 * clamp(r, 0, .999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0, .999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0, .999)) << '\n';
}