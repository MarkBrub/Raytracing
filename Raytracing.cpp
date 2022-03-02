#include "render.hpp"

int main() {
	render r;

	r.establish_camera();
	//r.generate_image();
	r.generate_image_multithreaded();
	r.render_to_bmp();
	r.render_to_ppm();

	return 0;
}