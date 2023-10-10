#include <vector>
namespace tsee::resources {
class Manager {
   private:
	std::vector<int> Textures;
	std::vector<int> Fonts;

   public:
	Manager();
	~Manager();
	void Unload();
};
}  // namespace tsee::resources