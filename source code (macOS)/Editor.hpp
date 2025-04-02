#ifndef Editor_hpp
#define Editor_hpp

#include "Tool.hpp"
#include "Config.hpp"
#include "Basic.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Editor {

void editor();

namespace UI {
int menu(int start, int end, const std::string& description, const std::string& operations);
bool yesNo(const std::string& description);
std::string str(const std::string& description, const std::string& item);
std::string line(const std::string& description, const std::string& item);
void alert(const std::string& description);
}

namespace Operation {
bool exit();
void editConfig();
void editCustom();
void resetConfig();
void resetCustom();
}

namespace Edit {
void editConfig(int mode);
void editCustom(int mode);
void createCustom();
bool deleteCustom(int mode);
void restoreDefault(Config& config);
int generateOperation(int index, std::unordered_map<int, int>& modeMap, Config& config, std::stringstream& operations);
void operationSelector(int operation, Config& config);
void editBool(bool& item, const std::string& str);
void editStr(std::string& item, const std::string& str1, const std::string& str2);
void editTxtURL(Config& config, const std::string& str1, const std::string& str2);
void editTxtName(Config& config, const std::string& str1, const std::string& str2);
}

}

#endif
