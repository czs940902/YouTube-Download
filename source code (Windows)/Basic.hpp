#ifndef Basic_hpp
#define Basic_hpp

#define VERSION   "4.0"
#define BUILDDATE "20250402"

#include "Config.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>

namespace Basic {

namespace Input {
bool getInt(int& n);
bool getWord(std::string& str);
bool getLine(std::string& str);
}

namespace Information {
void printInfo();
void printCustom();
void printModes();
void printArguments();
}

namespace Parse {
int input(int argc, const char *argv[], Config& config);
int mode(int argc, const char *argv[], Config& config);
bool arg(int argc, const char *argv[], Config& config);
}
}

#endif
