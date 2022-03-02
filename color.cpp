#include "color.hpp"

std::tuple<uint8_t, uint8_t, uint8_t> get_normalized_color(const color& pixel_color, const int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);

	return std::make_tuple(
		static_cast<uint8_t>(256 * clamp(r, 0, .999)),
		static_cast<uint8_t>(256 * clamp(g, 0, .999)),
		static_cast<uint8_t>(256 * clamp(b, 0, .999)));
}