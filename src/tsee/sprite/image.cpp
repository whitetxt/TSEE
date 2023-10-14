#include "../tsee.hpp"

using namespace tsee::sprite;

Image::Image() {
	this->w = -1;
	this->h = -1;
	this->path = "";
	this->textures = {};
}

Image::Image(std::string path) {
	this->surf = IMG_Load(path.c_str());
	if (!this->surf) {
		tsee::log::error(fmt::format("Couldn't load surface {}: ", path, IMG_GetError()));
		return;
	}
	this->w = this->surf->w;
	this->h = this->surf->h;
	this->path = path;
	this->textures = {};
}

Image::~Image() {
	for (auto kv : this->textures) {
		SDL_DestroyTexture(kv.second);
	}
	SDL_FreeSurface(this->surf);
}

void Image::Render(SDL_Renderer *rend, int x, int y) {
	SDL_Texture *tex;
	if (this->textures.find(rend) == this->textures.end()) {
		this->textures[rend] = SDL_CreateTextureFromSurface(rend, this->surf);
	} 
	tex = this->textures[rend];
	SDL_Rect rect = {x, y, this->w, this->h};
	SDL_RenderCopy(rend, tex, NULL, &rect);
}

void Image::SetPath(std::string path) {
	this->surf = IMG_Load(path.c_str());
	if (!this->surf) {
		tsee::log::error(fmt::format("Couldn't load surface {}: ", path, IMG_GetError()));
		return;
	}
	this->w = this->surf->w;
	this->h = this->surf->h;
	this->path = path;
	std::vector<SDL_Renderer*> windows;
	for (auto kv : this->textures) {
		windows.push_back(kv.first);
		SDL_DestroyTexture(kv.second);
	}
	this->textures = {};
	for (SDL_Renderer* rend : windows) {
		this->textures[rend] = SDL_CreateTextureFromSurface(rend, this->surf);
	}
}