#pragma once

#include <fstream>
#include <chrono>
#include <tuple>
#include <functional>

#include "bvh.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "util.hpp"
#include "bmp.hpp"
#include "pool.hpp"

struct render {
	//Final Product
	std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> image;

	//Image
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 1080;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1000;
	const int max_depth = 10;

	//World
	hittable_list world;

	//Camera
	point3 lookfrom;
	point3 lookat;
	vec3 vup;
	double dist_to_focus = 10.0;
	double aperture = 0.1;
	camera cam;

	void establish_camera();

	hittable_list random_scene();
	hittable_list default_scene();

	void generate_image();
	void generate_image_multithreaded();
	void render_to_ppm();
	void render_to_bmp();

private:
	color ray_color(const ray& r, const hittable& world, const int depth);
	color pixel_color(const int i, const int j);
};