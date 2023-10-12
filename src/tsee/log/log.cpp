#include "../tsee.hpp"

using namespace tsee::log;

std::string path = "main.log";

namespace tsee::log {
void generic_log(std::string type, std::string msg) {
	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	std::string out = fmt::format("{:%d-%m-%Y %T} - [{}] {}\n", t, type, msg);
	std::ofstream logfile;
	logfile.open(path, std::ios_base::app);
	logfile << out;
	logfile.close();
	std::cout << out;
}

void debug(std::string msg) {
	generic_log("DEBUG", msg);
}

void log(std::string msg) {
	generic_log("LOG", msg);
}

void warn(std::string msg) {
	generic_log("WARN", msg);
}

void error(std::string msg) {
	generic_log("ERROR", msg);
}
}  // namespace tsee::log