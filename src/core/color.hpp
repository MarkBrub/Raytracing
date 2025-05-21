#pragma once

#include <iostream>
#include <tuple>

#include "core/vec3.hpp"

std::tuple<uint8_t, uint8_t, uint8_t> get_normalized_color(const color& pixel_color, const int samples_per_pixel);