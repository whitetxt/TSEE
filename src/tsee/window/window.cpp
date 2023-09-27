#include "../tsee.hpp"

using namespace tsee::window;

int Window::Construct(int width, int height, std::string title) {
	int ret = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ret = -1;
		goto end;
	}
	this->window = SDL_CreateWindow(title.length() > 0 ? title.c_str() : "TSEE", SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!this->window) {
		ret = -2;
		goto end;
	}
	this->width = width;
	this->height = height;
	this->title = title.length() > 0 ? title : "TSEE";
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!this->renderer) {
		ret = -3;
		goto end;
	}
	this->vsync = true;
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
	this->last_render = SDL_GetPerformanceCounter();

	if (SDL_GetCurrentDisplayMode(0, &this->mode) != 0) {
		tsee::log::warn(fmt::format("Failed to get display mode ({})\n", SDL_GetError()));
		this->fps = 60;
	} else {
		this->fps = this->mode.refresh_rate;
		if (this->fps == 0) {
			tsee::log::warn("Unspecified refresh rate.\n");
			this->fps = 60;
		}
	}

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
	this->Construct(800, 640, nullptr);
}

Window::Window(int width, int height) {
	this->Construct(width, height, nullptr);
}

Window::Window(int width, int height, std::string title) {
	this->Construct(width, height, title);
}

void Window::Destroy() {
	tsee::log::debug(fmt::format("Destroying window: {}", this->title));
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Window::Render() {
	SDL_RenderPresent(this->renderer);
	this->last_render = SDL_GetPerformanceCounter();
}

SDL_Window *Window::GetSDLWindow() {
	return this->window;
}

SDL_Renderer *Window::GetSDLRenderer() {
	return this->renderer;
}

void Window::SetSize(int width, int height) {
	SDL_SetWindowSize(this->window, width, height);
	this->width = width;
	this->height = height;
}

SDL_Point Window::GetSize() {
	SDL_Point point;
	point.x = this->width;
	point.y = this->height;
	return point;
}

void Window::SetTitle(std::string title) {
	SDL_SetWindowTitle(this->window, title.c_str());
	this->title = title;
}

std::string Window::GetTitle() {
	return this->title;
}