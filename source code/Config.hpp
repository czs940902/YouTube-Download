#ifndef Config_hpp
#define Config_hpp

#include "Basic.hpp"
#include "Tool.hpp"
#include "Updater.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

struct Config0 {
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
    
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
    
    bool operator == (const Config0& other) const;
    bool operator != (const Config0& other) const;
    void reset();
    void initialize(int modeTemplate);
    void setTxtURL(const std::string& str);
    void setTxtName(const std::string& str);
};

extern std::string dlconfigPath;
extern std::string dlcustomPath;
extern std::array<Config0, 7> dlconfig;
extern std::array<Config0, 7> defaultConfig;
extern std::array<Config0, 7> initialConfig;
extern std::vector<Config0> dlcustom;
extern std::vector<Config0> defaultCustom;
extern std::vector<Config0> initialCustom;

namespace Config {

namespace RW {
void initialize();
void saveChanges();
bool loadConfig();
bool loadCustom();
bool saveConfig();
bool saveCustom();
}

namespace Check {
int searchCustom(const std::string& modeName);
bool isConfigDefault();
bool isConfigEdited();
bool isCustomDefault();
bool isCustomEdited();
}

}
#endif
