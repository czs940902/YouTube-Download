#include "Basic.hpp"

namespace Basic {

namespace Input {
bool getInt(int& n) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    int value; char extra;
    if(ss >> value && !(ss >> extra)) {
        n = value; return true;
    }
    return false;
}
bool getWord(std::string& str) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string word; char extra;
    if(ss >> word && !(ss >> extra)) {
        str = word; return true;
    }
    return false;
}
bool getLine(std::string& str) {
    std::string input;
    std::getline(std::cin, input);
    str = input;
    return true;
}
}

namespace FileRW {
void readInt(std::ifstream& file, int& value) {
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
}
void readBool(std::ifstream& file, bool& value) {
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
}
void readStr(std::ifstream& file, std::string& str) {
    size_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    str.resize(len);
    file.read(&str[0], len);
}
void writeInt(std::ofstream& file, int value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
}
void writeBool(std::ofstream& file, bool value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
}
void writeStr(std::ofstream& file, const std::string& str) {
    size_t len = str.size();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(str.data(), len);
}
}

}
