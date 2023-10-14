#include <iostream>

namespace tsee::log {
    extern bool isDebug;
    void debug(std::string msg);
    void log(std::string msg);
    void warn(std::string msg);
    void error(std::string msg);
}  // namespace tsee::log