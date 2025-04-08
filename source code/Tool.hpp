#ifndef Tool_hpp
#define Tool_hpp

#if defined(__APPLE__) && defined(__MACH__)
    #define MACOS
#elif defined(_WIN64)
    #if defined(__x86_64__) || defined(_M_X64)
        #define WINX64
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define WINA64
    #else
        #error "Unknown 64-bit platform"
    #endif
#else
    #error "Unsupported platform"
#endif

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
