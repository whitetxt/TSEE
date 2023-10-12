#include "../tsee.hpp"

using namespace tsee::sprite;

Image::Image(std::string path) {
	// if ((this->tex = tsee::resource::image::get(path))) {
	// }
	this->surf = IMG_Load(path.c_str());
	if (!this->surf) {
		tsee::log::error(fmt::format("Couldn't load surface {}: ", path, IMG_GetError()));
		return;
	}
	this->rect.w = this->surf->w;
	this->rect.h = this->surf->h;
	this->path = path;
}

Image::~Image() {
	for (std::pair kv : this->textures) {
		SDL_DestroyTexture(kv.second);
	}
	SDL_FreeSurface(this->surf);
}