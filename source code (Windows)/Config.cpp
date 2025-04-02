#include "Config.hpp"

std::string dlconfigPath;
std::string dlcustomPath;
std::array<Config, 7> dlconfig;
std::array<Config, 7> defaultConfig;
std::array<Config, 7> initialConfig;
std::vector<Config> dlcustom;
std::vector<Config> defaultCustom;
std::vector<Config> initialCustom;

bool Config::operator == (const Config& other) const {
    return  modeInt         == other.modeInt        &&
            modeStr         == other.modeStr        &&
            modeName        == other.modeName       &&
            keepOriginal    == other.keepOriginal   &&
            doNotConvert    == other.doNotConvert   &&
            customFileName  == other.customFileName &&
            printCommand    == other.printCommand   &&
            outputFormat    == other.outputFormat   &&
            audioCodec      == other.audioCodec     &&
            videoCodec      == other.videoCodec     &&
            addIndex        == other.addIndex       &&
            txtURL          == other.txtURL         &&
            txtName         == other.txtName        &&
            isTxtURLSet     == other.isTxtURLSet    &&
            isTxtNameSet    == other.isTxtNameSet;
}
bool Config::operator != (const Config& other) const {
    return !(*this == other);
}

void Config::resetter() {
    modeInt         = 0;
    modeStr         = "";
    modeName        = "";
    keepOriginal    = false;
    doNotConvert    = false;
    customFileName  = false;
    printCommand    = false;
    outputFormat    = "";
    audioCodec      = "";
    videoCodec      = "";
    addIndex        = false;
    txtURL          = "";
    txtName         = "";
    isTxtURLSet     = false;
    isTxtNameSet    = false;
}
void Config::initializer(int modeTemplate) {
    modeInt = modeTemplate;
    switch(modeTemplate) {
        case 1:
            modeStr         = "a";
            modeName        = "a";
            outputFormat    = "mp3";
            break;
        case 2:
            modeStr         = "v";
            modeName        = "v";
            outputFormat    = "mov";
            audioCodec      = "aac";
            videoCodec      = "libx264";
            break;
        case 3:
            modeStr         = "t";
            modeName        = "t";
            break;
        case 4:
            modeStr         = "fa";
            modeName        = "fa";
            outputFormat    = "mp3";
            txtURL          = "url.txt";
            txtName         = "name.txt";
            isTxtURLSet     = true;
            isTxtNameSet    = true;
            break;
        case 5:
            modeStr         = "fv";
            modeName        = "fv";
            outputFormat    = "mov";
            audioCodec      = "aac";
            videoCodec      = "libx264";
            txtURL          = "url.txt";
            txtName         = "name.txt";
            isTxtURLSet     = true;
            isTxtNameSet    = true;
            break;
        case 6:
            modeStr         = "ft";
            modeName        = "ft";
            txtURL          = "url.txt";
            txtName         = "name.txt";
            isTxtURLSet     = true;
            isTxtNameSet    = true;
            break;
        default: break;
    }
}

void Config::setTxtURL(const std::string& str) {
    if(str == "") { isTxtURLSet = false; txtURL = ""; }
    else          { isTxtURLSet = true; txtURL = str; }
}
void Config::setTxtName(const std::string& str) {
    if(str == "") { isTxtNameSet = false; txtName = ""; }
    else          { isTxtNameSet = true; txtName = str; }
}

void Config::serialize(std::ofstream& file) const {
    FileRW::writeInt(file, modeInt);
    FileRW::writeStr(file, modeStr);
    FileRW::writeStr(file, modeName);
    FileRW::writeBool(file, keepOriginal);
    FileRW::writeBool(file, doNotConvert);
    FileRW::writeBool(file, customFileName);
    FileRW::writeBool(file, printCommand);
    FileRW::writeStr(file, outputFormat);
    FileRW::writeStr(file, audioCodec);
    FileRW::writeStr(file, videoCodec);
    FileRW::writeBool(file, addIndex);
    FileRW::writeStr(file, txtURL);
    FileRW::writeStr(file, txtName);
    FileRW::writeBool(file, isTxtURLSet);
    FileRW::writeBool(file, isTxtNameSet);
}
void Config::deserialize(std::ifstream& file) {
    FileRW::readInt(file, modeInt);
    FileRW::readStr(file, modeStr);
    FileRW::readStr(file, modeName);
    FileRW::readBool(file, keepOriginal);
    FileRW::readBool(file, doNotConvert);
    FileRW::readBool(file, customFileName);
    FileRW::readBool(file, printCommand);
    FileRW::readStr(file, outputFormat);
    FileRW::readStr(file, audioCodec);
    FileRW::readStr(file, videoCodec);
    FileRW::readBool(file, addIndex);
    FileRW::readStr(file, txtURL);
    FileRW::readStr(file, txtName);
    FileRW::readBool(file, isTxtURLSet);
    FileRW::readBool(file, isTxtNameSet);
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

namespace ConfigRW {
void initialize() {
    dlconfigPath = Tool::getHomePath() + "/dlconfig.dat";
    dlcustomPath = Tool::getHomePath() + "/dlcustom.dat";
    bool isdlconfigExist = loadConfig(dlconfigPath);
    bool isdlcustomExist = loadCustom(dlcustomPath);
    for(int i=1;i<7;i++) defaultConfig[i].initializer(i);
    if(!isdlconfigExist) dlconfig = defaultConfig;
    if(!isdlcustomExist) dlcustom = defaultCustom;
    initialConfig = dlconfig;
    initialCustom = dlcustom;
}
void saveChanges() {
    if(ConfigCheck::isConfigEdited()) {
        if(ConfigCheck::isConfigDefault()) std::remove(dlconfigPath.c_str());
        else                               saveConfig(dlconfigPath);
    }
    if(ConfigCheck::isCustomEdited()) {
        if(ConfigCheck::isCustomDefault()) std::remove(dlcustomPath.c_str());
        else                               saveCustom(dlcustomPath);
    }
}
bool saveConfig(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if(!file.is_open()) return false;
    for(Config& config : dlconfig)
        config.serialize(file);
    file.close(); return true;
}
bool saveCustom(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if(!file.is_open()) return false;
    size_t customSize = dlcustom.size();
    file.write(reinterpret_cast<const char*>(&customSize), sizeof(customSize));
    for(Config& config : dlcustom)
        config.serialize(file);
    file.close(); return true;
}
bool loadConfig(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) return false;
    for(Config& config : dlconfig)
        config.deserialize(file);
    file.close(); return true;
}
bool loadCustom(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) return false;
    size_t customSize;
    file.read(reinterpret_cast<char*>(&customSize), sizeof(customSize));
    dlcustom.resize(customSize);
    for(Config& config : dlcustom)
        config.deserialize(file);
    file.close(); return true;
}
}

namespace ConfigCheck {
int searchCustom(const std::string& modeName) {
    size_t countCustom = dlcustom.size();
    for(int index=0;index<countCustom;index++) {
        if(modeName == dlcustom[index].modeName)
            return index;
    }
    return -1;
}
bool isConfigDefault() {
    return dlconfig == defaultConfig;
}
bool isConfigEdited() {
    return dlconfig != initialConfig;
}
bool isCustomDefault() {
    return dlcustom == defaultCustom;
}
bool isCustomEdited() {
    return dlcustom != initialCustom;
}
}
