#ifndef ConfigUpdater_hpp
#define ConfigUpdater_hpp

#define CURRENT_VERSION 1

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

namespace Updater {

namespace FileRW {
void readInt(std::ifstream& file, int& value);
void readBool(std::ifstream& file, bool& value);
void readStr(std::ifstream& file, std::string& str);
void writeInt(std::ofstream& file, int value);
void writeBool(std::ofstream& file, bool value);
void writeStr(std::ofstream& file, const std::string& str);
}

struct ConfigVer1 {
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
};

namespace Config {
extern int dlconfigVersion;
extern std::ifstream inputConfig;
extern std::ofstream outputConfig;
extern std::array<ConfigVer1, 7> dlconfig1;

bool check(const std::string& filePath);
void loadFile();
void convert();
void saveFile();
void load1();
}

namespace Custom {
extern int dlcustomVersion;
extern std::ifstream inputCustom;
extern std::ofstream outputCustom;
extern std::vector<ConfigVer1> dlcustom1;

bool check(const std::string& filePath);
void loadFile();
void convert();
void saveFile();
void load1();
}

}

#endif
