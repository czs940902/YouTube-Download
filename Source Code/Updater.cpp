#include "Updater.hpp"

namespace Updater {

void Config1::serialize(std::ofstream &file) const {
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
void Config2::serialize(std::ofstream &file) const {
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
void Config1::deserialize(std::ifstream &file) {
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
void Config2::deserialize(std::ifstream &file) {
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

std::array<Config1, 7> dlconfig1;
std::array<Config2, 7> dlconfig2;
std::vector<Config1> dlcustom1;
std::vector<Config2> dlcustom2;

namespace Config {
bool update(int versionConfig, std::ifstream& inputConfig, const std::string& dlconfigPath) {
    switch(versionConfig) {
        case 1: load1(inputConfig); break;
        case 2: load2(inputConfig); break;
        default: break;
    }
    switch(versionConfig) {
        case 1: convert1to2();
        default: break;
    }
    
    std::ofstream outputConfig(dlconfigPath, std::ios::binary);
    if(!outputConfig.is_open()) return false;
    Basic::FileRW::writeInt(outputConfig, CONFIG_VER);
    for(Config2& config : dlconfig2) config.serialize(outputConfig);
    outputConfig.close();
    
    int checkVersionConfig; std::string temp;
    inputConfig.open(dlconfigPath, std::ios::binary);
    if(!inputConfig.is_open()) return false;
    Basic::FileRW::readInt(inputConfig, checkVersionConfig);
    if(checkVersionConfig != CONFIG_VER) return false;
    return true;
}
void load1(std::ifstream& inputConfig) {
    for(Config1& config : dlconfig1) config.deserialize(inputConfig);
    inputConfig.close();
}
void load2(std::ifstream& inputConfig) {
    for(Config2& config : dlconfig2) config.deserialize(inputConfig);
    inputConfig.close();
}
void convert1to2() {
    for(int i=0;i<7;i++) {
        dlconfig2[i].modeInt        = dlconfig1[i].modeInt;
        dlconfig2[i].modeStr        = dlconfig1[i].modeStr;
        dlconfig2[i].modeName       = dlconfig1[i].modeName;
        dlconfig2[i].keepOriginal   = dlconfig1[i].keepOriginal;
        dlconfig2[i].doNotConvert   = dlconfig1[i].doNotConvert;
        dlconfig2[i].customFileName = dlconfig1[i].customFileName;
        dlconfig2[i].printCommand   = dlconfig1[i].printCommand;
        dlconfig2[i].outputFormat   = dlconfig1[i].outputFormat;
        dlconfig2[i].audioCodec     = dlconfig1[i].audioCodec;
        dlconfig2[i].videoCodec     = dlconfig1[i].videoCodec;
        dlconfig2[i].addIndex       = dlconfig1[i].addIndex;
        dlconfig2[i].txtURL         = dlconfig1[i].txtURL;
        dlconfig2[i].txtName        = dlconfig1[i].txtName;
        dlconfig2[i].isTxtURLSet    = dlconfig1[i].isTxtURLSet;
        dlconfig2[i].isTxtNameSet   = dlconfig1[i].isTxtNameSet;
    }
}
}

namespace Custom {
bool update(int versionCustom, std::ifstream& inputCustom, const std::string& dlcustomPath) {
    switch(versionCustom) {
        case 1: load1(inputCustom); break;
        case 2: load2(inputCustom); break;
        default: break;
    }
    switch(versionCustom) {
        case 1: convert1to2();
        default: break;
    }
    
    std::ofstream outputCustom(dlcustomPath, std::ios::binary);
    if(!outputCustom.is_open()) return false;
    Basic::FileRW::writeInt(outputCustom, CONFIG_VER);
    size_t customSize = dlcustom2.size();
    outputCustom.write(reinterpret_cast<const char*>(&customSize), sizeof(customSize));
    for(Config2& config : dlcustom2) config.serialize(outputCustom);
    outputCustom.close();
    
    int checkVersionCustom; std::string temp;
    inputCustom.open(dlcustomPath, std::ios::binary);
    if(!inputCustom.is_open()) return false;
    Basic::FileRW::readInt(inputCustom, checkVersionCustom);
    if(checkVersionCustom != CONFIG_VER) return false;
    return true;
}
void load1(std::ifstream& inputCustom) {
    size_t customSize;
    inputCustom.read(reinterpret_cast<char*>(&customSize), sizeof(customSize));
    dlcustom1.resize(customSize);
    for(Config1& config : dlcustom1) config.deserialize(inputCustom);
    inputCustom.close();
}
void load2(std::ifstream& inputCustom) {
    size_t customSize;
    inputCustom.read(reinterpret_cast<char*>(&customSize), sizeof(customSize));
    dlcustom2.resize(customSize);
    for(Config2& config : dlcustom2) config.deserialize(inputCustom);
    inputCustom.close();
}
void convert1to2() {
    for(Config1& config : dlcustom1) {
        Config2 temp;
        temp.modeInt        = config.modeInt;
        temp.modeStr        = config.modeStr;
        temp.modeName       = config.modeName;
        temp.keepOriginal   = config.keepOriginal;
        temp.doNotConvert   = config.doNotConvert;
        temp.customFileName = config.customFileName;
        temp.printCommand   = config.printCommand;
        temp.outputFormat   = config.outputFormat;
        temp.audioCodec     = config.audioCodec;
        temp.videoCodec     = config.videoCodec;
        temp.addIndex       = config.addIndex;
        temp.txtURL         = config.txtURL;
        temp.txtName        = config.txtName;
        temp.isTxtURLSet    = config.isTxtURLSet;
        temp.isTxtNameSet   = config.isTxtNameSet;
        dlcustom2.emplace_back(temp);
    }
}
}

}
