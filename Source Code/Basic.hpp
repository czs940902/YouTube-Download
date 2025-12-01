#ifndef Basic_hpp
#define Basic_hpp

#define VERSION    "4.2.1"
#define BUILDDATE  "20251201"
#define CONFIG_VER 1

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace Basic {

namespace Input {
bool getInt(int& n);
bool getWord(std::string& str);
void getLine(std::string& str);
void getEnter();
}

namespace FileRW {
void readInt(std::ifstream& file, int& value);
void readBool(std::ifstream& file, bool& value);
void readStr(std::ifstream& file, std::string& str);
void writeInt(std::ofstream& file, int value);
void writeBool(std::ofstream& file, bool value);
void writeStr(std::ofstream& file, const std::string& str);
}

}

#endif
