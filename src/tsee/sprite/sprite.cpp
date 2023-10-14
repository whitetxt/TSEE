#include "../tsee.hpp"

using namespace tsee::sprite;

Sprite::Sprite(SDL_Renderer *rend, std::string path) {
	this->image = Image(path);
    this->rend = rend;
    this->x = 0;
    this->y = 0;
}

void Sprite::Render() {
    this->image.Render(this->rend, this->x, this->y);
}