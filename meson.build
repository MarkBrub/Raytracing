#meson build

project('raytracer', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'optimization=3',
    'buildtype=release',
    'b_lto=true',
    'cpp_args=-march=native -ffast-math -fno-math-errno -funroll-loops -fprefetch-loop-arrays -fno-rtti',
    'werror=true'
  ]
)


src = files(
            'src/raytracing.cpp',
            'src/acceleration/aabb.cpp',
            'src/acceleration/bvh.cpp',
            'src/acceleration/improved_bvh.cpp',
            'src/core/color.cpp',
            'src/core/vec3.cpp',
            'src/geometry/aa_rect.cpp',
            'src/geometry/box.cpp',
            'src/geometry/sphere.cpp',
            'src/render/bmp.cpp',
            'src/render/render.cpp',
            'src/scene/hittable.cpp',
            'src/scene/hittable_list.cpp',
            'src/utils/pool.cpp',
            'src/volumes/constant_medium.cpp')

executable(meson.project_name(),
    sources : src,
    include_directories : include_directories('src')
)