#ifndef Main_hpp
#define Main_hpp

#include "Basic.hpp"
#include "Config.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>

namespace Main {

namespace Information {
void printInfo();
void printCustom();
void printModes();
void printArguments();
}

namespace Parse {
int input(int argc, const char *argv[], Config0& config);
int mode(int argc, const char *argv[], Config0& config);
bool arg(int argc, const char *argv[], Config0& config);
}

}

#endif
