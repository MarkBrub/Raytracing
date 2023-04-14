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
#include "scene.hpp"

struct render {
	// Final Product
	std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> image;

	// Image
	// double aspect_ratio = 16.0 / 9.0;
	double aspect_ratio = 1.0;
	int image_width = 1920 / 4;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	int samples_per_pixel = 100;
	int max_depth = 50;

	// World
	hittable_list world;

	// Camera
	camera cam;

	// Background color
	// Takes in a vec3 and returns a color
	std::function<color(const vec3&)> background;

	render() {
		switch (4) {
			case 0: 
				init_scene(scene::default_scene);
				break;
			case 1:
				init_scene(scene::random_scene);
				break;
			case 2:
				init_scene(scene::basic_light);
				break;
			case 3:
				init_scene(scene::simple_light);
				break;
			case 4:
				init_scene(scene::basic_cornell_box);
				break;
			case 5:
				init_scene(scene::smoke_cornell_box);
				break;
			case 6:
				init_scene(scene::final_scene);
				break;
			default:
				// Throw a runtime error
				throw std::runtime_error("Invalid scene selected");
				break;
		}
	}

	// Takes a function from the scene class and calls it then moves the values into the render class
	// The return type is a tuple of a hittable_list, camera, and a function that takes a vec3 and returns a color
	// The function takes in the aspect ratio of the image
	void init_scene(std::function<std::tuple<hittable_list, camera, std::function<color(const vec3&)>>(double)> scene_func) {
		auto [w, c, b] = scene_func(aspect_ratio);

		world = std::move(w);
		cam = std::move(c);
		background = std::move(b);
	}

	void generate_image();
	void generate_image_multithreaded();
	void render_to_ppm();
	void render_to_bmp();

private:
	color ray_color(const ray& r, const std::function<color(const vec3&)>& background, const hittable& world, const int depth);
	color pixel_color(const int i, const int j);
};