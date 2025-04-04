#ifndef Tool_hpp
#define Tool_hpp

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

namespace Tool {
void cleanScreen();
std::string getHomePath();
std::string getTargetOS();
bool runCommand(const std::string& command, bool printCommand);
bool downloadJPG(const std::string url, const std::string fileName, bool printCommand);
void update(bool printCommand);
}

#endif
