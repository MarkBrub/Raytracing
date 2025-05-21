#include "render/render.hpp"

int main() {
	render renderer;

	//renderer.generate_image();
	renderer.generate_image_multithreaded();
	renderer.render_to_bmp();
	renderer.render_to_ppm();

	return 0;
}