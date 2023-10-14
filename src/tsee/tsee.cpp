#include "tsee.hpp"

using namespace tsee;

TSEE::TSEE() {
	typedef std::chrono::high_resolution_clock c;
	std::chrono::time_point last_time = c::now();
	std::chrono::duration since_epoch = last_time.time_since_epoch();
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch);
	this->last_time = microseconds.count();
	this->current_time = microseconds.count();
	this->dt = 0;
	this->last_window_id = 0;
	this->LoadSettings();
	return;
}

TSEE::~TSEE() {
	// Clear settings
	std::ofstream file;
	file.open("settings.ini");
	file.close();
	// Each window will save its sizes
	for (window::Window w : this->windows) {
		w.Destroy();
	}
	this->SaveSettings();
}

void TSEE::SettingsCallback(std::string line) {
	if (line[0] == '#') {
		log::debug("[Settings] Skipping commented line.");
		return;
	}
	std::string sec = line.substr(0, line.find("="));
	std::string val = line.substr(line.find("=") + 1);
	log::debug(fmt::format("[Settings] Got line: {}={}", sec, val));
	if (sec == "debug") {
		this->SetDebug(val.compare("true") == 0);
	} else {
		log::warn(fmt::format("[SETTINGS] Unknown section: {} with value {}", sec, val));
	}
}

void TSEE::LoadSettings() {
	std::ifstream file;
	file.open("tsee.ini");
	std::string line;
	while (std::getline(file, line)) {
		this->SettingsCallback(line);
	}
	file.close();
}

void TSEE::SaveSettings() {
	std::ofstream file;
	file.open("tsee.ini");
	file << fmt::format("debug={}\n", this->isDebug ? "true" : "false");
	file.close();
}

void TSEE::SetDebug(bool debug) {
	this->isDebug = debug;
	log::isDebug = debug;
}

window::Window TSEE::CreateWindow(int width, int height, std::string title) {
	window::Window win(++this->last_window_id, width, height, title);
	this->windows.push_back(win);
	return win;
}

window::Window TSEE::CreateWindow(int width, int height) {
	window::Window win(++this->last_window_id, width, height);
	this->windows.push_back(win);
	return win;
}

window::Window TSEE::CreateWindow() {
	window::Window win(++this->last_window_id);
	this->windows.push_back(win);
	return win;
}

void TSEE::RenderAll() {
	for (window::Window w : this->windows) {
		w.Render();
	}
}