#pragma once

namespace tsee::window {
class Window {
   private:
	int Construct();

	SDL_Window *window;
	SDL_Renderer *renderer;

	int width;
	int height;

	std::string title;

   public:
	Window();
	Window(int width, int height);
	Window(int width, int height, std::string title);

	void Render();
	void Destroy();
};
}  // namespace tsee::window