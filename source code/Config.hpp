#ifndef Config_hpp
#define Config_hpp

#define CONFIG_VERSION 1

#include "Tool.hpp"
#include "ConfigUpdater.hpp"

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <cstdio>

struct Config {
    int             modeInt         = 0;
    std::string     modeStr         = "";
    std::string     modeName        = "";
    bool            keepOriginal    = false;
    bool            doNotConvert    = false;
    bool            customFileName  = false;
    bool            printCommand    = false;
    std::string     outputFormat    = "";
    std::string     audioCodec      = "";
    std::string     videoCodec      = "";
    bool            addIndex        = false;
    std::string     txtURL          = "";
    std::string     txtName         = "";
    bool            isTxtURLSet     = false;
    bool            isTxtNameSet    = false;
    
    bool operator == (const Config& other) const;
    bool operator != (const Config& other) const;
    
    void resetter();
    void initializer(int modeTemplate);
    
    void setTxtURL(const std::string& str);
    void setTxtName(const std::string& str);
    
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
};


extern std::ifstream inputConfig;
extern std::ifstream inputCustom;
extern std::ofstream outputConfig;
extern std::ofstream outputCustom;
extern std::string dlconfigPath;
extern std::string dlcustomPath;
extern std::array<Config, 7> dlconfig;
extern std::array<Config, 7> defaultConfig;
extern std::array<Config, 7> initialConfig;
extern std::vector<Config> dlcustom;
extern std::vector<Config> defaultCustom;
extern std::vector<Config> initialCustom;

namespace FileRW {
void readInt(std::ifstream& file, int& value);
void readBool(std::ifstream& file, bool& value);
void readStr(std::ifstream& file, std::string& str);
void writeInt(std::ofstream& file, int value);
void writeBool(std::ofstream& file, bool value);
void writeStr(std::ofstream& file, const std::string& str);
}

namespace ConfigRW {
void initialize();
void saveChanges();
bool saveConfig(const std::string& filename);
bool saveCustom(const std::string& filename);
bool loadConfig(const std::string& filename);
bool loadCustom(const std::string& filename);
}

namespace ConfigCheck {
int searchCustom(const std::string& modeName);
bool isConfigDefault();
bool isConfigEdited();
bool isCustomDefault();
bool isCustomEdited();
}

#endif
