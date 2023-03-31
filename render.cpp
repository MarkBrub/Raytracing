#include "render.hpp"

void render::establish_camera() {
	lookfrom = point3(13, 2, 3);
	lookat = point3(0, 0, 0);
	vup = vec3(0, 1, 0);

	cam = camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
}

hittable_list render::random_scene() {
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for(int a = -11; a < 11; a++) {
		for(int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if(choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				} else if(choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
	world.add(std::make_shared<sphere>(point3(0, 1, 0), -.9, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	// retun here to use the original scene without bvh
	// return world;

	bvh_node bvh_tree = create_bvh_tree(world);
	hittable_list bvh_world;
	bvh_world.add(std::make_shared<bvh_node>(bvh_tree));
	return bvh_world;
}

hittable_list render::default_scene() {
	return hittable_list();
}

void render::generate_image() {
	// Render
	auto start = std::chrono::high_resolution_clock::now();

	world = random_scene();

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

	world = random_scene();

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
		print_progress_bar(static_cast<double>(total_tasks) - p.remaining_tasks.load(), total_tasks);
	}

	p.join_threads();

	for(auto& pixel : results) {
		image.push_back(get_normalized_color(pixel.get(), samples_per_pixel));
	}

	std::cerr << "\nDone.\n";

	auto time = std::chrono::high_resolution_clock::now() - start;
	std::cerr << "Image generated in " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << "ms" << std::endl;
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

color render::ray_color(const ray& r, const hittable& world, const int depth) {
	hit_record rec;
	if(depth <= 0) return color(0, 0, 0);
	if(world.hit(r, .001, infinity, rec)) {
		ray scattered;
		color attenuation;

		if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return color(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction);
	auto t = .5 * (unit_direction.y() + 1);
	return ((1.0 - t) * color(1, 1, 1)) + (t * color(.5, .7, 1));
}

color render::pixel_color(const int i, const int j) {
	color pixel_color(0, 0, 0);
	for(int x = 0; x < samples_per_pixel; x++) {
		auto u = double(i + random_double()) / (double(image_width) - 1);
		auto v = double(j + random_double()) / (double(image_height) - 1);

		ray r = cam.get_ray(u, v);
		pixel_color += ray_color(r, world, max_depth);
	}

	return pixel_color;
}