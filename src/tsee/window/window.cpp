#include "../tsee.hpp"

using namespace tsee::window;

int Window::Construct(int id, int width, int height, std::string title) {
	this->id = id;
	int ret = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ret = -1;
		goto end;
	}
	this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
									SDL_WINDOW_SHOWN);
	if (!this->window) {
		ret = -2;
		goto end;
	}
	this->width = width;
	this->height = height;
	this->title = title;
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
    this->LoadSettings();
	return ret;
}

Window::Window(int id) {
	this->Construct(id, 800, 640, "TSEE");
}

Window::Window(int id, int width, int height) {
	this->Construct(id, width, height, "TSEE");
}

Window::Window(int id, int width, int height, std::string title) {
	this->Construct(id, width, height, title);
}

void Window::Destroy() {
	tsee::log::debug(fmt::format("Destroying window: {}", this->title));
	this->SaveSettings();
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Window::Render() {
	if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_MINIMIZED) {
		// Don't render if minimised.
		// Limits to ~15 FPS
		SDL_Delay(67);
	}
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);

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

void Window::SettingsCallback(std::string line) {
	if (line[0] == '#') {
		log::debug("[Settings] Skipping commented line.");
		return;
	}
	std::string sec = line.substr(0, line.find("="));
	std::string val = line.substr(line.find("=") + 1);
	log::debug(fmt::format("[Settings] Got line: {}={}", sec, val));
	if (sec == "width") {
		this->SetSize(std::stoi(val), this->height);
	} else if (sec == "height") {
		this->SetSize(this->width, std::stoi(val));
	} else {
		log::warn(fmt::format("[SETTINGS] Unknown section: {} with value {}", sec, val));
	}
}

void Window::LoadSettings() {
	std::ifstream file;
	file.open("settings.ini");
	std::string line;
	std::string window_name;
	while (std::getline(file, line)) {
		if (line[0] == '>') {
			window_name = line.substr(1);
			continue;
		}
		if (window_name != this->title) {
			continue;
		}
		this->SettingsCallback(line);
	}
	file.close();
}

void Window::SaveSettings() {
	std::ofstream file;
	file.open("settings.ini");
	file << fmt::format(">{}\n", this->title);
	file << fmt::format("width={}\n", this->width);
	file << fmt::format("height={}\n", this->height);
	file.close();
}

int Window::GetID() {
	return this->id;
}