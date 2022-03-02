#include <fstream>
#include <vector>
#include <tuple>
#include <ranges>

class bmp {
	//Header
	const char* signature = "BM";
	uint32_t file_size = 0;
	const uint32_t reserved = 0;
	const uint32_t data_offset = 54;

	//Info Header
	const uint32_t header_size = 40;
	uint32_t width = 0;
	uint32_t height = 0;
	const uint16_t planes = 1;
	const uint16_t bits_per_pixel = 24;
	const uint32_t compression = 0;
	const uint32_t image_size = 0;
	const uint32_t x_pixels_per_m = 0;
	const uint32_t y_pixels_per_m = 0;
	const uint32_t colors_used = 0;
	const uint32_t important_colors = 0;

	void write_header(std::ofstream& out);
	void write_info_header(std::ofstream& out);

	template <typename T>
	inline void write_to_stream(std::ofstream& stream, const T& t) const {
		stream.write(reinterpret_cast<const char*>(&t), sizeof(T));
	}

public:
	bmp(const int image_width, const int image_height) : width(image_width), height(image_height), file_size(54 + (image_width * image_height)) {}
	void write_to_file(std::ofstream& out, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> pixels);
};