#include "../tsee.hpp"

using namespace tsee::window;

int Window::Construct() {
	int ret = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ret = -1;
		goto end;
	}
	this->window = SDL_CreateWindow("TSEE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);
	if (!this->window) {
		ret = -2;
		goto end;
	}
	this->renderer = SDL_CreateRenderer(this->window, -1, 0);
	if (!this->renderer) {
		ret = -3;
		goto end;
	}
	this->width = 800;
	this->height = 640;
	this->title = "TSEE";

end:
	switch (ret) {
		case 0:
			break;
		case -1:
			tsee::log::error(fmt::format("Failed to initialise SDL.\nError -> {}", SDL_GetError()));
			break;
		case -2:
			tsee::log::error(fmt::format("Failed to create window.\nError: {}", SDL_GetError()));
			break;
		case -3:
			tsee::log::error(fmt::format("Failed to create renderer.\nError: {}", SDL_GetError()));
			break;
		default:
			tsee::log::warn(fmt::format("Unexpected return code from Window::Construct -> {}", ret));
			break;
	}
	return ret;
}

Window::Window() {
	this->Construct();
}

Window::Window(int width, int height) {
	this->Construct();
	SDL_SetWindowSize(this->window, width, height);
	this->width = width;
	this->height = height;
}

Window::Window(int width, int height, std::string title) {
	this->Construct();
	SDL_SetWindowSize(this->window, width, height);
	this->width = width;
	this->height = height;
	SDL_SetWindowTitle(this->window, title.c_str());
	this->title = title;
}

void Window::Destroy() {
	tsee::log::log(fmt::format("Destroying window: {}", this->title));
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Window::Render() {
	SDL_RenderPresent(this->renderer);
}