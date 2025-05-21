#pragma once

#include <tuple>

#include "scene/hittable_list.hpp"
#include "camera/camera.hpp"
#include "core/color.hpp"
#include "geometry/sphere.hpp"
#include "geometry/aa_rect.hpp"
#include "geometry/box.hpp"
#include "volumes/constant_medium.hpp"
#include "materials/material.hpp"

class scene {
public:

	// each function returns a hittable_list world and a camera cam
	// all functions are static

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> simple_light(double aspect_ratio) {

		hittable_list objects;

		color c(.5, .5, .5);
		objects.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(c)));
		objects.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(c)));

		auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
		objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

		// Camera setup
		point3 lookfrom = point3(26, 3, 6);
		point3 lookat = point3(0, 2, 0);
		vec3 vup(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(0, 0, 0);
		};

		return std::make_tuple(objects, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> basic_light(double aspect_ratio) {
		// A scene with a single sphere and a rectangle light source
		hittable_list world;

		auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
		world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

		auto material1 = std::make_shared<dielectric>(1.5);
		world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

		auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
		world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

		auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.3);
		world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

		auto light = std::make_shared<diffuse_light>(color(30, 30, 30));
		world.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, light));

		// Camera setup
		point3 lookfrom(13, 2, 3);
		point3 lookat(0, 0, 0);
		vec3 vup(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(0, 0, 0);
		};

		return std::make_tuple(world, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> random_scene(double aspect_ratio) {
		hittable_list world;

		auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
		world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_double();
				point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

				if ((center - point3(4, 0.2, 0)).length() > 0.9) {
					std::shared_ptr<material> sphere_material;

					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = color::random() * color::random();
						sphere_material = std::make_shared<lambertian>(albedo);
						world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = color::random(0.5, 1);
						auto fuzz = random_double(0, 0.5);
						sphere_material = std::make_shared<metal>(albedo, fuzz);
						world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
					}
					else if (choose_mat < 0.99) {
						// glass
						sphere_material = std::make_shared<dielectric>(1.5);
						world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
					}
					else {
						// light
						sphere_material = std::make_shared<diffuse_light>(color(5, 5, 5));
						world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
					}
				}
			}
		}

		// This needs to be made a hittable_list so the bvh properly handles
		// the inverted sphere insdide the first sphere
		auto material1 = std::make_shared<dielectric>(1.5);
		hittable_list hollow_glass_ball;
		hollow_glass_ball.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
		hollow_glass_ball.add(std::make_shared<sphere>(point3(0, 1, 0), -0.8, material1));
		world.add(std::make_shared<hittable_list>(hollow_glass_ball));

		// Switch between matte and light
		auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
		// auto material2 = std::make_shared<diffuse_light>(color(30, 30, 30));
		world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

		auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
		world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

		// Camera setup
		point3 lookfrom = point3(13, 2, 3);
		point3 lookat = point3(0, 0, 0);
		vec3 vup = vec3(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			// Blend betweeen the background and white based on angle
			vec3 unit_direction = unit_vector(d);
			auto t = .5 * (unit_direction.y() + 1);
			color blend(.70, .80, 1.00); // Sky blue

			return ((1.0 - t) * color(1, 1, 1)) + (t * blend);
		};

		return std::make_tuple(world, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> default_scene(double aspect_ratio) {
		hittable_list world;
		auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
		world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
		auto material1 = std::make_shared<dielectric>(1.5);
		world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
		auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
		world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
		auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
		world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

		// Camera setup
		point3 lookfrom = point3(13, 2, 3);
		point3 lookat = point3(0, 0, 0);
		vec3 vup = vec3(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(.70, .80, 1.00); // Sky blue
		};

		return std::make_tuple(world, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> basic_cornell_box(double aspect_ratio) {
		hittable_list objects;

		auto red = std::make_shared<lambertian>(color(.65, .05, .05));
		auto white = std::make_shared<lambertian>(color(.73, .73, .73));
		auto green = std::make_shared<lambertian>(color(.12, .45, .15));
		auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

		// Walls of the cornell box
		objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
		objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));

		objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
		objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
		objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));

		objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

		// Two boxes withing the cornell box
		std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
		box1 = std::make_shared<rotate_y>(box1, 15);
		box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));
		objects.add(box1);

		std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
		box2 = std::make_shared<rotate_y>(box2, -18);
		box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));
		objects.add(box2);

		// Camera setup
		point3 lookfrom(278, 278, -800);
		point3 lookat(278, 278, 0);
		vec3 vup(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.0;

		camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(0, 0, 0);
		};

		return std::make_tuple(objects, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> smoke_cornell_box(double aspect_ratio) {
		hittable_list objects;

		auto red = std::make_shared<lambertian>(color(.65, .05, .05));
		auto white = std::make_shared<lambertian>(color(.73, .73, .73));
		auto green = std::make_shared<lambertian>(color(.12, .45, .15));
		auto light = std::make_shared<diffuse_light>(color(30, 30, 30));

		// Walls of the cornell box
		objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
		objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));

		objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
		objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
		objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));

		objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

		// Two boxes within the cornell box
		std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
		box1 = std::make_shared<rotate_y>(box1, 15);
		box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));

		std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
		box2 = std::make_shared<rotate_y>(box2, -18);
		box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));

		objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
		objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

		// Camera setup
		point3 lookfrom(278, 278, -800);
		point3 lookat(278, 278, 0);
		vec3 vup(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.0;

		camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(0, 0, 0);
		};

		return std::make_tuple(objects, cam, background);
	}

	static std::tuple<hittable_list, camera, std::function<color(const vec3&)>> final_scene(double aspect_ratio) {
		hittable_list objects;
		auto ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

		const int boxes_per_side = 20;
		for (int i = 0; i < boxes_per_side; i++) {
			for (int j = 0; j < boxes_per_side; j++) {
				auto w = 100.0;
				auto x0 = -1000.0 + i * w;
				auto z0 = -1000.0 + j * w;
				auto y0 = 0.0;
				auto x1 = x0 + w;
				auto y1 = random_double(1, 101);
				auto z1 = z0 + w;

				objects.add(std::make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
			}
		}

		auto light = std::make_shared<diffuse_light>(color(7, 7, 7));
		objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

		auto moving_sphere_material = std::make_shared<metal>(color(0.7, 0.3, 0.1), 0);
		objects.add(std::make_shared<sphere>(point3(400, 400, 200), 50, moving_sphere_material));

		objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
		objects.add(std::make_shared<sphere>(point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

		auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70, std::make_shared<dielectric>(1.5));
		objects.add(boundary);
		objects.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
		boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
		objects.add(std::make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

		// auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("earthmap.jpg"));
		auto green = std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
		objects.add(std::make_shared<sphere>(point3(400, 200, 400), 100, green));
		//auto pertext = std::make_shared<noise_texture>(0.1);
		auto pink = std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
		objects.add(std::make_shared<sphere>(point3(220, 280, 300), 80, pink));

		hittable_list boxes2;
		auto white = std::make_shared<lambertian>(color(.73, .73, .73));
		int ns = 1000;
		for (int j = 0; j < ns; j++) {
			boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
		}

		objects.add(std::make_shared<translate>(
			std::make_shared<rotate_y>(
				std::make_shared<bvh_node>(boxes2), 15),
				vec3(-100, 270, 395)
			)
		);

		// Camera setup
		point3 lookfrom(478, 278, -600);
		point3 lookat(278, 278, 0);
		vec3 vup(0, 1, 0);
		double dist_to_focus = 10.0;
		double aperture = 0.0;

		camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);

		// Background color
		auto background = [](const vec3& d) -> color {
			return color(0, 0, 0);
		};

		return std::make_tuple(objects, cam, background);
	}
};