#pragma once

#include "utils/util.hpp"
#include "core/vec3.hpp"
#include "core/ray.hpp"

struct hit_record;

struct material {
	virtual color emitted() const {
		return color(0, 0, 0);
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// A lambertian is a material that absorbs light.
// These will render as matte surfaces.
struct lambertian : public material {
	color albedo;

	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		if(scatter_direction.near_zero()) scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		//Note we could just as well only scatter with some probability p and have attenuation be albedo/p. Your choice.

		return true;
	}
};

// A metal is a material that reflects light.
// These will render as shiny surfaces.
struct metal : public material {
	color albedo;
	// needs to be between 0 and 1
	double fuzz;

	metal(const color& a, const double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction), rec.normal);

		scattered = ray(rec.p, reflected + (fuzz * random_in_unit_sphere()));
		attenuation = albedo;
		//Note we could just as well only scatter with some probability p and have attenuation be albedo/p. Your choice.

		return dot(scattered.direction, rec.normal) > 0;
	}
};

// A dielectric is a material that refracts light.
// These will render as transparent surfaces.
struct dielectric : public material {
	// Index of refraction
	double ir;

	dielectric(const double index_of_refraction) : ir(index_of_refraction) {}

	// Snell's Law
	// This scatters the rays through the dielectric.
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1, 1, 1);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction);
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1);
		double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

		bool cannot_refract = refraction_ratio * sin_theta > 1;
		vec3 direction;

		if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
			direction = reflect(unit_direction, rec.normal);
		} else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		return true;
	}

private:
	static double reflectance(const double cosine, const double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

// A diffuse light is a material that emits light.
// These will render as light sources.
class diffuse_light : public material {
public:
	diffuse_light(std::shared_ptr<color> a) : emit(a) {}
	diffuse_light(const color& c) : emit(std::make_shared<color>(c)) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		return false;
	}

	// Because I don't have textures yet, I'm just going to return the color of the light.
	virtual color emitted() const override {
		return *emit;
	}

	private:
		std::shared_ptr<color> emit;
};

class isotropic : public material {
public:
	isotropic(const color& c) : albedo(std::make_shared<color>(c)) {}
	
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		scattered = ray(rec.p, random_in_unit_sphere());
		attenuation = *albedo;
		return true;
	}

public:
	std::shared_ptr<color> albedo;
};