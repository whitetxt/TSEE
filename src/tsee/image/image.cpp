#include "../tsee.hpp"

using namespace tsee::image;

void Texture::Construct() {
	this->rect = (SDL_Rect){0, 0, 0, 0};
}

Texture::Texture() {
	this->Construct();
}

Texture::Texture(std::string path) {
	this->Construct();
	this->SetPath(path);
}

Texture::~Texture() {}

void Texture::SetPath(std::string path) {
	this->path = path;
}