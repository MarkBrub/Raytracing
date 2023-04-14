#include "render.hpp"

void render::generate_image() {
	// Render
	auto start = std::chrono::high_resolution_clock::now();

	for(int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for(int i = 0; i < image_width; ++i) {
			color c = pixel_color(i, j);
			image.push_back(get_normalized_color(c, samples_per_pixel));
		}
	}

	std::cerr << "\nDone.\n";

	auto time = std::chrono::high_resolution_clock::now() - start;
	std::cerr << "Image generated in " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << "ms" << std::endl;
}

void render::generate_image_multithreaded() {
	// Render
	auto start = std::chrono::high_resolution_clock::now();

	pool p;
	std::vector<std::future<color>> results;

	for(int j = image_height - 1; j >= 0; --j) {
		for(int i = 0; i < image_width; ++i) {
			results.push_back(p.enqueue_task(&render::pixel_color, this, i, j));
		}
	}

	p.start_pool();

	std::cout << "Threads started" << std::endl;
	std::cout << "Total pixels: " << p.remaining_tasks.load() << std::endl;

	const int total_tasks = image_height * image_width;
	
	while(p.remaining_tasks.load()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		auto elapsed_time = std::chrono::high_resolution_clock::now() - start;
		auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();

		print_progress_bar(static_cast<double>(total_tasks) - p.remaining_tasks.load(), total_tasks, elapsed_time_ms);
	}

	p.join_threads();

	for(auto& pixel : results) {
		image.push_back(get_normalized_color(pixel.get(), samples_per_pixel));
	}

	std::cerr << "\nDone.\n";

	auto time = std::chrono::high_resolution_clock::now() - start;
	auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	std::cerr << "Image generated in " << ms_to_time(time_ms) << std::endl;
}

void render::render_to_ppm() {
	// Render
	std::ofstream out("render.ppm");

	out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Create a string buffer to store the pixel data
	std::stringstream buffer;

	for(const auto& pixel : image) {
		buffer << (int)std::get<0>(pixel) << " " << (int)std::get<1>(pixel) << " " << (int)std::get<2>(pixel) << '\n';
	}

	// Write the entire buffer to the file in one operation
	out << buffer.rdbuf();

	out.close();
}

void render::render_to_bmp() {
	std::ofstream out("render.bmp", std::ios::binary);
	bmp file(image_width, image_height);

	file.write_to_file(out, image);

	out.close();
}

color render::ray_color(const ray& r, const std::function<color(const vec3&)>& background, const hittable& world, const int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered
	if(depth <= 0) return color(0, 0, 0);

	// If the ray doesn't hit anything, return the background color
	if (!world.hit(r, 0.001, infinity, rec)) {
		return background(r.direction);
	}

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted();

	if(!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
		return emitted;
	}

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

color render::pixel_color(const int i, const int j) {
	color pixel_color(0, 0, 0);
	for(int x = 0; x < samples_per_pixel; x++) {
		auto u = double(i + random_double()) / (double(image_width) - 1);
		auto v = double(j + random_double()) / (double(image_height) - 1);

		ray r = cam.get_ray(u, v);
		pixel_color += ray_color(r, background, world, max_depth);
	}

	return pixel_color;
}