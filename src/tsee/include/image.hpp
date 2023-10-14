namespace tsee::sprite {
class Image {
   private:
	SDL_Surface *surf;
	SDL_Rect rect;
	std::string path;
	std::unordered_map<int, SDL_Texture*> textures;

   public:
	Image(std::string path);
	~Image();
};
}  // namespace tsee::sprite
