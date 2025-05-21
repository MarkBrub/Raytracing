#include "render/bmp.hpp"
#include <ranges>
#include <ranges>

void bmp::write_header(std::ofstream& out) {
	out.write(signature, 2);
	out.write(reinterpret_cast<const char*>(&file_size), 4);
	out.write(reinterpret_cast<const char*>(&reserved), 4);
	out.write(reinterpret_cast<const char*>(&data_offset), 4);
}

void bmp::write_info_header(std::ofstream& out) {
	out.write(reinterpret_cast<const char*>(&header_size), 4);
	out.write(reinterpret_cast<const char*>(&width), 4);
	out.write(reinterpret_cast<const char*>(&height), 4);
	out.write(reinterpret_cast<const char*>(&planes), 2);
	out.write(reinterpret_cast<const char*>(&bits_per_pixel), 2);
	out.write(reinterpret_cast<const char*>(&compression), 4);
	out.write(reinterpret_cast<const char*>(&image_size), 4);
	out.write(reinterpret_cast<const char*>(&x_pixels_per_m), 4);
	out.write(reinterpret_cast<const char*>(&y_pixels_per_m), 4);
	out.write(reinterpret_cast<const char*>(&colors_used), 4);
	out.write(reinterpret_cast<const char*>(&important_colors), 4);

}

void bmp::write_to_file(std::ofstream& out, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>& pixels) {
    write_header(out);
    write_info_header(out);

    // Calculate the padding needed for each row
    std::size_t rowSize = width * 3;
    std::size_t paddingSize = (4 - (rowSize % 4)) % 4;

    // Calculate the total size of the pixel data including padding
    std::size_t dataSize = (rowSize + paddingSize) * height;

    // Create a buffer to store the pixel data
    std::vector<uint8_t> buffer;
    buffer.reserve(dataSize);

    // Reverse iterator to loop through the pixel rows in reverse
    auto rowIter = pixels.rbegin();

    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            const auto& pixel = *rowIter;
            buffer.push_back(std::get<2>(pixel)); // Blue
            buffer.push_back(std::get<1>(pixel)); // Green
            buffer.push_back(std::get<0>(pixel)); // Red
            ++rowIter;
        }
        // Add padding to the row
        for (std::size_t pad = 0; pad < paddingSize; ++pad) {
            buffer.push_back(0);
        }
    }

    // Write the entire buffer to the file in one operation
    out.write(reinterpret_cast<const char*>(buffer.data()), dataSize);
}

