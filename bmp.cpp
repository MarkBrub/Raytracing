#include "bmp.hpp"
#include <ranges>

void bmp::write_header(std::ofstream& out) {
	out.write(signature, 2);
	write_to_stream(out, file_size);
	write_to_stream(out, reserved);
	write_to_stream(out, data_offset);
}

void bmp::write_info_header(std::ofstream& out) {
	write_to_stream(out, header_size);
	write_to_stream(out, width);
	write_to_stream(out, height);
	write_to_stream(out, planes);
	write_to_stream(out, bits_per_pixel);
	write_to_stream(out, compression);
	write_to_stream(out, image_size);
	write_to_stream(out, x_pixels_per_m);
	write_to_stream(out, y_pixels_per_m);
	write_to_stream(out, colors_used);
	write_to_stream(out, important_colors);
}

void bmp::write_to_file(std::ofstream& out, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> pixels) {
	write_header(out);
	write_info_header(out);

	//This will have to wait till visual studio loves me
	//auto v = pixels | std::ranges::views::reverse;

	for(auto it = pixels.rbegin(); it < pixels.rend(); it++) {
		// The colors in a BMP are stored in reverse order
		write_to_stream(out, std::get<2>(*it));
		write_to_stream(out, std::get<1>(*it));
		write_to_stream(out, std::get<0>(*it));
	}
}
