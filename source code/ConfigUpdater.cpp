#include "ConfigUpdater.hpp"

namespace Updater {

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

namespace Config {
int dlconfigVersion;
std::ifstream inputConfig;
std::ofstream outputConfig;
std::array<ConfigVer1, 7> dlconfig1;

bool check(const std::string& filePath) {
    std::string temp;
    inputConfig.open(filePath, std::ios::binary);
    if(!inputConfig.is_open()) return false;
    Updater::FileRW::readInt(inputConfig, dlconfigVersion);
    if(dlconfigVersion == CURRENT_VERSION) { inputConfig.close(); return true; }
    if(dlconfigVersion > CURRENT_VERSION) {
        std::cout << "Unsupported dlconfig.dat file detected!\n"
                  << "Press enter to load default configs...\n";
        std::getline(std::cin, temp);
        return false;
    } else {
        std::cout << "Outdated dlconfig.dat file detected!\n"
                  << "Press enter to update and reload...\n";
        std::getline(std::cin, temp);
        loadFile(); convert(); inputConfig.close();
        outputConfig.open(filePath, std::ios::binary);
        if(!outputConfig.is_open()) return false;
        saveFile(); outputConfig.close();
        return true;
    }
}
void loadFile() {
    switch(dlconfigVersion) {
        case 1: load1(); break;
        default: break;
    }
}
void convert() {
    switch(dlconfigVersion) {
        default: break;
    }
}
void saveFile() {
    Updater::FileRW::writeInt(outputConfig, CURRENT_VERSION);
    for(ConfigVer1& config : dlconfig1) {
        Updater::FileRW::writeInt(outputConfig, config.modeInt);
        Updater::FileRW::writeStr(outputConfig, config.modeStr);
        Updater::FileRW::writeStr(outputConfig, config.modeName);
        Updater::FileRW::writeBool(outputConfig, config.keepOriginal);
        Updater::FileRW::writeBool(outputConfig, config.doNotConvert);
        Updater::FileRW::writeBool(outputConfig, config.customFileName);
        Updater::FileRW::writeBool(outputConfig, config.printCommand);
        Updater::FileRW::writeStr(outputConfig, config.outputFormat);
        Updater::FileRW::writeStr(outputConfig, config.audioCodec);
        Updater::FileRW::writeStr(outputConfig, config.videoCodec);
        Updater::FileRW::writeBool(outputConfig, config.addIndex);
        Updater::FileRW::writeStr(outputConfig, config.txtURL);
        Updater::FileRW::writeStr(outputConfig, config.txtName);
        Updater::FileRW::writeBool(outputConfig, config.isTxtURLSet);
        Updater::FileRW::writeBool(outputConfig, config.isTxtNameSet);
    }
}
void load1() {
    for(ConfigVer1& config : dlconfig1) {
        Updater::FileRW::readInt(inputConfig, config.modeInt);
        Updater::FileRW::readStr(inputConfig, config.modeStr);
        Updater::FileRW::readStr(inputConfig, config.modeName);
        Updater::FileRW::readBool(inputConfig, config.keepOriginal);
        Updater::FileRW::readBool(inputConfig, config.doNotConvert);
        Updater::FileRW::readBool(inputConfig, config.customFileName);
        Updater::FileRW::readBool(inputConfig, config.printCommand);
        Updater::FileRW::readStr(inputConfig, config.outputFormat);
        Updater::FileRW::readStr(inputConfig, config.audioCodec);
        Updater::FileRW::readStr(inputConfig, config.videoCodec);
        Updater::FileRW::readBool(inputConfig, config.addIndex);
        Updater::FileRW::readStr(inputConfig, config.txtURL);
        Updater::FileRW::readStr(inputConfig, config.txtName);
        Updater::FileRW::readBool(inputConfig, config.isTxtURLSet);
        Updater::FileRW::readBool(inputConfig, config.isTxtNameSet);
    }
}
}

namespace Custom {
int dlcustomVersion;
std::ifstream inputCustom;
std::ofstream outputCustom;
std::vector<ConfigVer1> dlcustom1;

bool check(const std::string& filePath) {
    std::string temp;
    inputCustom.open(filePath, std::ios::binary);
    if(!inputCustom.is_open()) return false;
    Updater::FileRW::readInt(inputCustom, dlcustomVersion);
    if(dlcustomVersion == CURRENT_VERSION) { inputCustom.close(); return true; }
    if(dlcustomVersion > CURRENT_VERSION) {
        std::cout << "Unsupported dlcustom.dat file detected!\n"
                  << "Press enter to load default configs...\n";
        std::getline(std::cin, temp);
        return false;
    } else {
        std::cout << "Outdated dlcustom.dat file detected!\n"
                  << "Press enter to update and reload...\n";
        std::getline(std::cin, temp);
        loadFile(); convert(); inputCustom.close();
        outputCustom.open(filePath, std::ios::binary);
        if(!outputCustom.is_open()) return false;
        saveFile(); outputCustom.close();
        return true;
    }
}
void loadFile() {
    switch(dlcustomVersion) {
        case 1: load1(); break;
        default: break;
    }
}
void convert() {
    switch(dlcustomVersion) {
        default: break;
    }
}
void saveFile() {
    Updater::FileRW::writeInt(outputCustom, CURRENT_VERSION);
    size_t customSize = dlcustom1.size();
    outputCustom.write(reinterpret_cast<const char*>(&customSize), sizeof(&customSize));
    for(ConfigVer1& config : dlcustom1) {
        Updater::FileRW::writeInt(outputCustom, config.modeInt);
        Updater::FileRW::writeStr(outputCustom, config.modeStr);
        Updater::FileRW::writeStr(outputCustom, config.modeName);
        Updater::FileRW::writeBool(outputCustom, config.keepOriginal);
        Updater::FileRW::writeBool(outputCustom, config.doNotConvert);
        Updater::FileRW::writeBool(outputCustom, config.customFileName);
        Updater::FileRW::writeBool(outputCustom, config.printCommand);
        Updater::FileRW::writeStr(outputCustom, config.outputFormat);
        Updater::FileRW::writeStr(outputCustom, config.audioCodec);
        Updater::FileRW::writeStr(outputCustom, config.videoCodec);
        Updater::FileRW::writeBool(outputCustom, config.addIndex);
        Updater::FileRW::writeStr(outputCustom, config.txtURL);
        Updater::FileRW::writeStr(outputCustom, config.txtName);
        Updater::FileRW::writeBool(outputCustom, config.isTxtURLSet);
        Updater::FileRW::writeBool(outputCustom, config.isTxtNameSet);
    }
}
void load1() {
    size_t customSize;
    inputCustom.read(reinterpret_cast<char*>(&customSize), sizeof(customSize));
    dlcustom1.resize(customSize);
    for(ConfigVer1& config : dlcustom1) {
        Updater::FileRW::readInt(inputCustom, config.modeInt);
        Updater::FileRW::readStr(inputCustom, config.modeStr);
        Updater::FileRW::readStr(inputCustom, config.modeName);
        Updater::FileRW::readBool(inputCustom, config.keepOriginal);
        Updater::FileRW::readBool(inputCustom, config.doNotConvert);
        Updater::FileRW::readBool(inputCustom, config.customFileName);
        Updater::FileRW::readBool(inputCustom, config.printCommand);
        Updater::FileRW::readStr(inputCustom, config.outputFormat);
        Updater::FileRW::readStr(inputCustom, config.audioCodec);
        Updater::FileRW::readStr(inputCustom, config.videoCodec);
        Updater::FileRW::readBool(inputCustom, config.addIndex);
        Updater::FileRW::readStr(inputCustom, config.txtURL);
        Updater::FileRW::readStr(inputCustom, config.txtName);
        Updater::FileRW::readBool(inputCustom, config.isTxtURLSet);
        Updater::FileRW::readBool(inputCustom, config.isTxtNameSet);
    }
}
}

}
