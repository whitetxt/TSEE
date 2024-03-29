namespace tsee {
class TSEE {
   public:
	int last_window_id;
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
	void LoadSettings();
	void SaveSettings();
	window::Window CreateWindow(int width, int height, std::string title);
	window::Window CreateWindow(int width, int height);
	window::Window CreateWindow();
	void SetDebug(bool debug);
	void RenderAll();

   private:
	int last_time;
	int current_time;
	bool isDebug;
	double dt;
	void SettingsCallback(std::string line);
};
}  // namespace tsee