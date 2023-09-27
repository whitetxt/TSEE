namespace tsee {
class TSEE {
   public:
	std::vector<window::Window> windows;
	// ResourceManager resources;
	// World world;
	// Events events;
	// Player player;
	// Init init;
	// UI ui;
	// Debug debug;
	// Animations animations;
	TSEE();
	~TSEE();
	window::Window CreateWindow(int width, int height, std::string title);
	window::Window CreateWindow(int width, int height);
	window::Window CreateWindow();
	void RenderAll();

   private:
	int last_time;
	int current_time;
	double dt;
};
}  // namespace tsee