#ifndef Main_hpp
#define Main_hpp

#include "Basic.hpp"
#include "Tool.hpp"
#include "Config.hpp"
#include "Updater.hpp"
#include "Editor.hpp"
#include "YouTube.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace Main {

namespace Information {
void printInfo();
void printCustom();
void printModes();
void printArguments();
}

namespace Parse {
int input(int argc, const char *argv[]);
int run(std::string& modeStr, std::vector<std::string>& args);
bool mode(std::string& modeStr, Config0& config);
bool arg(std::vector<std::string>& args, Config0& config);
}

}

#endif
