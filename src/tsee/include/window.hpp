#pragma once

namespace tsee::window {
class Window {
   private:
	int id;

	SDL_Window *window;
	SDL_Renderer *renderer;

	int x;
	int y;
	int width;
	int height;
	int fps;
	bool vsync;
	std::string title;

	SDL_Point mouse;
	SDL_DisplayMode mode;
	Uint64 last_render;

	int Construct(int id, int width, int height, std::string title);
	void SettingsCallback(std::string line);

   public:
	Window(int id);
	Window(int id, int width, int height);
	Window(int id, int width, int height, std::string title);

	void Render();
	void Destroy();
	SDL_Window *GetSDLWindow();
	SDL_Renderer *GetSDLRenderer();
	void SetSize(int width, int height);
	SDL_Point GetSize();
	void SetTitle(std::string title);
	std::string GetTitle();
	void LoadSettings();
	void SaveSettings();
	int GetID();
};
}  // namespace tsee::window