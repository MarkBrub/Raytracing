#include "volumes/constant_medium.hpp"

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1)) return false;

    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2)) return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    rec1.t = std::max(rec1.t, t_min);
    rec2.t = std::min(rec2.t, t_max);

    if (rec1.t >= rec2.t) return false;

    rec1.t = std::max(rec1.t, 0.0);

    const auto ray_length = r.direction.length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary) return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) std::cerr << "hit_distance = " << hit_distance << '\n' << "rec.t = " << rec.t << '\n' << "rec.p = " << rec.p << '\n';

    rec.normal = vec3(1, 0, 0);  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}