namespace tsee::sprite {
class Image {
   private:
	SDL_Surface *surf;
	int w;
	int h;
	std::string path;
	std::unordered_map<SDL_Renderer*, SDL_Texture*> textures;

   public:
    Image();
	Image(std::string path);
	~Image();
	void SetPath(std::string path);
	void Render(SDL_Renderer *rend, int x, int y);
};

class Sprite {
   private:
	Image image;
	SDL_Renderer *rend;

	int x;
	int y;

   public:
	Sprite(SDL_Renderer *rend, std::string path);
	void Render();
};
}  // namespace tsee::sprite
