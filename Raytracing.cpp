#include <fstream>
#include <chrono>

#include "util.hpp"

#include "color.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

color ray_color(const ray& r, const hittable& world, const int depth) {
	hit_record rec;
	if(depth <= 0) return color(0, 0, 0);
	if(world.hit(r, .001, infinity, rec)) {
		point3 target = rec.p + rec.normal + random_unit_vector();
		return .5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
	}
	vec3 unit_direction = unit_vector(r.direction);
	auto t = .5 * (unit_direction.y() + 1);
	return ((1.0 - t) * color(1, 1, 1)) + (t * color(.5, .7, 1));
}

int main() {
	auto start = std::chrono::high_resolution_clock::now();
	std::ofstream out("render.ppm");

	//Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 20;
	const int max_depth = 10;

	//World
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

	//Camera
	camera cam;

	// Render
	out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for(int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for(int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for(int x = 0; x < samples_per_pixel; x++) {
				auto u = double(i + random_double()) / (double(image_width) - 1);
				auto v = double(j + random_double()) / (double(image_height) - 1);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}

			write_color(out, pixel_color, samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";

	out.close();

	auto time = std::chrono::high_resolution_clock::now() - start;
	std::cerr << "Program run in " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << "ms" << std::endl;

	return 0;
}