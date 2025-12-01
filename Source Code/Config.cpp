#include "Config.hpp"

void Config0::serialize(std::ofstream& file) const {
    Basic::FileRW::writeInt(file, modeInt);
    Basic::FileRW::writeStr(file, modeStr);
    Basic::FileRW::writeStr(file, modeName);
    Basic::FileRW::writeBool(file, keepOriginal);
    Basic::FileRW::writeBool(file, doNotConvert);
    Basic::FileRW::writeBool(file, customFileName);
    Basic::FileRW::writeBool(file, printCommand);
    Basic::FileRW::writeStr(file, outputFormat);
    Basic::FileRW::writeStr(file, audioCodec);
    Basic::FileRW::writeStr(file, videoCodec);
    Basic::FileRW::writeBool(file, addIndex);
    Basic::FileRW::writeStr(file, txtURL);
    Basic::FileRW::writeStr(file, txtName);
    Basic::FileRW::writeBool(file, isTxtURLSet);
    Basic::FileRW::writeBool(file, isTxtNameSet);
}
void Config0::deserialize(std::ifstream& file) {
    Basic::FileRW::readInt(file, modeInt);
    Basic::FileRW::readStr(file, modeStr);
    Basic::FileRW::readStr(file, modeName);
    Basic::FileRW::readBool(file, keepOriginal);
    Basic::FileRW::readBool(file, doNotConvert);
    Basic::FileRW::readBool(file, customFileName);
    Basic::FileRW::readBool(file, printCommand);
    Basic::FileRW::readStr(file, outputFormat);
    Basic::FileRW::readStr(file, audioCodec);
    Basic::FileRW::readStr(file, videoCodec);
    Basic::FileRW::readBool(file, addIndex);
    Basic::FileRW::readStr(file, txtURL);
    Basic::FileRW::readStr(file, txtName);
    Basic::FileRW::readBool(file, isTxtURLSet);
    Basic::FileRW::readBool(file, isTxtNameSet);
}

bool Config0::operator == (const Config0& other) const {
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
bool Config0::operator != (const Config0& other) const {
    return !(*this == other);
}
void Config0::reset() {
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
void Config0::initialize(int modeTemplate) {
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
void Config0::setTxtURL(const std::string& str) {
    if(str == "") { isTxtURLSet = false; txtURL = ""; }
    else          { isTxtURLSet = true; txtURL = str; }
}
void Config0::setTxtName(const std::string& str) {
    if(str == "") { isTxtNameSet = false; txtName = ""; }
    else          { isTxtNameSet = true; txtName = str; }
}

std::string dlconfigPath;
std::string dlcustomPath;
std::array<Config0, 7> dlconfig;
std::array<Config0, 7> defaultConfig;
std::array<Config0, 7> initialConfig;
std::vector<Config0> dlcustom;
std::vector<Config0> defaultCustom;
std::vector<Config0> initialCustom;

namespace Config {

namespace RW {
void initialize() {
    for(int i=1;i<7;i++) defaultConfig[i].initialize(i);
    dlconfigPath = Tool::getHomePath() + "/dlconfig.dat";
    dlcustomPath = Tool::getHomePath() + "/dlcustom.dat";
    if(!loadConfig()) dlconfig = defaultConfig;
    if(!loadCustom()) dlcustom = defaultCustom;
    initialConfig = dlconfig;
    initialCustom = dlcustom;
}
void saveChanges() {
    if(Config::Check::isConfigEdited()) {
        if(Config::Check::isConfigDefault())
            std::remove(dlconfigPath.c_str());
        else saveConfig();
    }
    if(Config::Check::isCustomEdited()) {
        if(Config::Check::isCustomDefault())
            std::remove(dlcustomPath.c_str());
        else saveCustom();
    }
}
bool loadConfig() {
    int versionConfig;
    std::ifstream inputConfig(dlconfigPath, std::ios::binary);
    if(!inputConfig.is_open()) return false;
    Basic::FileRW::readInt(inputConfig, versionConfig);
    if(versionConfig > CONFIG_VER) {
        std::cout << "Unsupported dlconfig.dat file detected!\n"
                  << "Press enter to load default configs...\n";
        Basic::Input::getEnter();
        inputConfig.close(); return false;
    }
    if(versionConfig < CONFIG_VER) {
        std::cout << "Outdated dlconfig.dat file detected!\n"
                  << "Press enter to update and reload...\n";
        Basic::Input::getEnter();
        if(!Updater::Config::update(versionConfig, inputConfig, dlconfigPath)) {
            inputConfig.close();
            std::cout << "Update failed!\n"
                      << "Press enter to load default configs...\n";
            Basic::Input::getEnter();
            return false;
        }
    }
    
    for(Config0& config : dlconfig) config.deserialize(inputConfig);
    inputConfig.close(); return true;
}
bool loadCustom() {
    int versionCustom;
    std::ifstream inputCustom(dlcustomPath, std::ios::binary);
    if(!inputCustom.is_open()) return false;
    Basic::FileRW::readInt(inputCustom, versionCustom);
    if(versionCustom > CONFIG_VER) {
        std::cout << "Unsupported dlcustom.dat file detected!\n"
                  << "Press enter to load default configs...\n";
        Basic::Input::getEnter();
        inputCustom.close(); return false;
    }
    if(versionCustom < CONFIG_VER) {
        std::cout << "Outdated dlcustom.dat file detected!\n"
                  << "Press enter to update and reload...\n";
        Basic::Input::getEnter();
        if(!Updater::Custom::update(versionCustom, inputCustom, dlcustomPath)) {
            inputCustom.close();
            std::cout << "Update failed!\n"
                      << "Press enter to load default configs...\n";
            Basic::Input::getEnter();
            return false;
        }
    }
    
    size_t customSize;
    inputCustom.read(reinterpret_cast<char*>(&customSize), sizeof(customSize));
    dlcustom.resize(customSize);
    for(Config0& config : dlcustom) config.deserialize(inputCustom);
    inputCustom.close(); return true;
}
bool saveConfig() {
    std::ofstream outputConfig(dlconfigPath, std::ios::binary);
    if(!outputConfig.is_open()) return false;
    Basic::FileRW::writeInt(outputConfig, CONFIG_VER);
    for(Config0& config : dlconfig) config.serialize(outputConfig);
    outputConfig.close(); return true;
}
bool saveCustom() {
    std::ofstream outputCustom(dlcustomPath, std::ios::binary);
    if(!outputCustom.is_open()) return false;
    Basic::FileRW::writeInt(outputCustom, CONFIG_VER);
    size_t customSize = dlcustom.size();
    outputCustom.write(reinterpret_cast<const char*>(&customSize), sizeof(customSize));
    for(Config0& config : dlcustom) config.serialize(outputCustom);
    outputCustom.close(); return true;
}
}

namespace Check {
int searchCustom(const std::string& modeName) {
    size_t countCustom = dlcustom.size();
    for(int index=0;index<countCustom;index++) {
        if(modeName == dlcustom[index].modeName)
            return index;
    }
    return -1;
}
bool isConfigDefault() { return dlconfig == defaultConfig; }
bool isConfigEdited() { return dlconfig != initialConfig; }
bool isCustomDefault() { return dlcustom == defaultCustom; }
bool isCustomEdited() { return dlcustom != initialCustom; }
}

}
