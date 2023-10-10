#include <SDL2/SDL.h>
#include <string>

namespace tsee::image {
class Texture {
   private:
	void Construct();
	SDL_Texture *texture;
	SDL_Rect rect;
	std::string path;

   public:
	Texture();
	Texture(std::string path);
	~Texture();
	void SetPath(std::string path);
};
}  // namespace tsee::image