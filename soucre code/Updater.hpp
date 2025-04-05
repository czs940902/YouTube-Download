#ifndef Updater_hpp
#define Updater_hpp

#include "Basic.hpp"

#include <string>
#include <fstream>
#include <array>
#include <vector>

namespace Updater {

struct Config1 {
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
};
struct Config2 {
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
};

extern std::array<Config1, 7> dlconfig1;
extern std::array<Config2, 7> dlconfig2;
extern std::vector<Config1> dlcustom1;
extern std::vector<Config2> dlcustom2;

namespace Config {
bool update(int versionConfig, std::ifstream& inputConfig, const std::string& dlconfigPath);
void load1(std::ifstream& inputConfig);
void load2(std::ifstream& inputConfig);
void convert1to2();
}

namespace Custom {
bool update(int versionCustom, std::ifstream& inputCustom, const std::string& dlcustomPath);
void load1(std::ifstream& inputCustom);
void load2(std::ifstream& inputCustom);
void convert1to2();
}

}

#endif
