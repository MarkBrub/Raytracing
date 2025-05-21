#pragma once

#include <fstream>
#include <chrono>
#include <tuple>
#include <functional>

#include "acceleration/bvh.hpp"
#include "core/vec3.hpp"
#include "core/ray.hpp"
#include "core/color.hpp"
#include "camera/camera.hpp"
#include "scene/hittable_list.hpp"
#include "geometry/sphere.hpp"
#include "materials/material.hpp"
#include "utils/util.hpp"
#include "render/bmp.hpp"
#include "utils/pool.hpp"
#include "scene/scene.hpp"
#include "acceleration/improved_bvh.hpp"

struct render {
	// Final Product
	std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> image;

	// Image
	// double aspect_ratio = 16.0 / 9.0;
	double aspect_ratio = 1.0;
	int image_width = 1920 / 8;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	int samples_per_pixel = 300;
	int max_depth = 4;

	// World
	hittable_list world;

	// Camera
	camera cam;

	// Background color
	// Takes in a vec3 and returns a color
	std::function<color(const vec3&)> background;

	render() {
		switch (6) {
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
		auto start = std::chrono::high_resolution_clock::now();

		auto [w, c, b] = scene_func(aspect_ratio);
		
		improved_bvh_node improved_bvh_world(w);
		world.add(std::make_shared<improved_bvh_node>(improved_bvh_world));
		cam = std::move(c);
		background = std::move(b);

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "Scene initialization took " << duration.count() << " microseconds" << std::endl;
	}

	void generate_image();
	void generate_image_multithreaded();
	void render_to_ppm();
	void render_to_bmp();

private:
	color ray_color(const ray& r, const std::function<color(const vec3&)>& background, const hittable& world, const int depth);
	color pixel_color(const int i, const int j);
};