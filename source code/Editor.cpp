#include "Editor.hpp"

namespace Editor {

void editor() {
    while(true) {
        std::stringstream description, operations;
        description << "Default edited ... " << (ConfigCheck::isConfigDefault() ? "NO" : "YES") << "\n"
                    << "Custom mode ...... " << (ConfigCheck::isCustomDefault() ? "NO" : "YES") << "\n";
        operations  << "[0]\tExit\n"
                    << "[1]\tEdit built-in modes\n"
                    << "[2]\tEdit custom modes\n"
                    << "[3]\tReset built-in modes\n"
                    << "[4]\tReset custom modes\n";
        switch(Editor::UI::menu(0, 4, description.str(), operations.str())) {
            case 0: if(Editor::Operation::exit()) return;   break;
            case 1: Editor::Operation::editConfig();        break;
            case 2: Editor::Operation::editCustom();        break;
            case 3: Editor::Operation::resetConfig();       break;
            case 4: Editor::Operation::resetCustom();       break;
            default: break;
        }
    }
}

namespace UI {
int menu(int start, int end, const std::string& description, const std::string& operations) {
    while(true) {
        Tool::cleanScreen();
        std::cout   << "*** EDIT MODE ***\n\n"
                    << description << "\n"
                    << operations << "\n"
                    << "Choose a operation: ";
        int operation;
        if(!Basic::Input::getInt(operation)) {
            alert("Invalid input!");
        } else if(operation < start || operation > end) {
            alert("Operation does not exist!");
        } else {
            Tool::cleanScreen();
            return operation;
        }
    }
}
bool yesNo(const std::string& description) {
    while(true) {
        Tool::cleanScreen();
        std::cout   << "*** EDIT MODE ***\n\n"
                    << description << "\n"
                    << "[0]\tNO\n[1]\tYES\n\n"
                    << "Choose a operation: ";
        int operation;
        if(!Basic::Input::getInt(operation)) {
            alert("Invalid input!");
        } else if(operation == 0) {
            Tool::cleanScreen();
            return false;
        } else if(operation == 1) {
            Tool::cleanScreen();
            return true;
        } else {
            alert("Operation does not exist!");
        }
    }
}
std::string str(const std::string& description, const std::string& item) {
    Tool::cleanScreen();
    std::cout   << "*** EDIT MODE ***\n\n"
                << description << "\n"
                << "Enter a " << item << ": ";
    std::string str;
    Basic::Input::getWord(str);
    Tool::cleanScreen();
    return str;
}
std::string line(const std::string& description, const std::string& item) {
    Tool::cleanScreen();
    std::cout   << "*** EDIT MODE ***\n\n"
                << description << "\n"
                << "Enter a " << item << ": ";
    std::string str;
    Basic::Input::getLine(str);
    Tool::cleanScreen();
    return str;
}
void alert(const std::string& description) {
    std::string temp;
    Tool::cleanScreen();
    std::cout   << "*** EDIT MODE ***\n\n"
                << description << "\n"
                << "Press enter to continue...\n";
    Basic::Input::getLine(temp);
    Tool::cleanScreen();
}
}

namespace Operation {
bool exit() {
    if(!ConfigCheck::isConfigEdited() && !ConfigCheck::isCustomEdited()) return true;
    std::stringstream description, operations;
    description << "Config edited, save or discard?\n";
    operations  << "[0]\tReturn\n"
                << "[1]\tSave\n"
                << "[2]\tDiscard\n";
    switch(Editor::UI::menu(0, 2, description.str(), operations.str())) {
        case 0:
            return false;
        case 1:
            ConfigRW::saveChanges();
            Editor::UI::alert("Changes saved!");
            return true;
        case 2:
            Editor::UI::alert("Changes discarded!");
            return true;
        default:
            return false;
    }
}
void editConfig() {
    while(true) {
        std::stringstream description, operations;
        description << "Edit built-in modes\n";
        operations  << "[0]\tReturn\n"
                    << "[1]\ta mode\n"
                    << "[2]\tv mode\n"
                    << "[3]\tt mode\n"
                    << "[4]\tfa mode\n"
                    << "[5]\tfv mode\n"
                    << "[6]\tft mode\n";
        int operation = Editor::UI::menu(0, 6, description.str(), operations.str());
        if(operation == 0) { Tool::cleanScreen(); return; }
        else Editor::Edit::editConfig(operation);
    }
}
void editCustom() {
    while(true) {
        int countCustomMode = (int)dlcustom.size();
        std::stringstream description, operations;
        description << "Edit custom mode\n";
        operations  << "[0]\tReturn\n"
                    << "[1]\tCreate a new custom mode\n";
        for(int i=0;i<countCustomMode;i++)
            operations << "[" << i+2 << "]\t" << dlcustom[i].modeName << "\n";
        int operation = Editor::UI::menu(0, countCustomMode+1, description.str(), operations.str());
        if(operation == 0) { Tool::cleanScreen(); return; }
        else if(operation == 1) Editor::Edit::createCustom();
        else Editor::Edit::editCustom(operation-2);
    }
}
void resetConfig() {
    if(Editor::UI::yesNo("Reset all built-in modes?\n")) {
        dlconfig = defaultConfig;
        Editor::UI::alert("Reset succeed!");
    }
}
void resetCustom() {
    if(Editor::UI::yesNo("Delete all custom modes?\n")) {
        dlcustom = defaultCustom;
        Editor::UI::alert("Reset succeed!");
    }
}
}

namespace Edit {
void editConfig(int mode) {
    while(true) {
        std::unordered_map<int, int> modeMap;
        std::stringstream description, operations;
        description << "Editing mode " << dlconfig[mode].modeName << "\n";
        operations  << "[0]\tReturn\n"
                    << "[1]\tReset to default\n";
        int index = generateOperation(2, modeMap, dlconfig[mode], operations);
        int operation = Editor::UI::menu(0, index, description.str(), operations.str());
        if(operation < 2) {
            switch(operation) {
                case 0: return;
                case 1: restoreDefault(dlconfig[mode]); break;
                default: break;
            }
        } else {
            operationSelector(modeMap[operation], dlconfig[mode]);
        }
    }
}
void editCustom(int mode) {
    while(true) {
        std::unordered_map<int, int> modeMap;
        std::stringstream description, operations;
        description << "Editing custom mode \"" << dlcustom[mode].modeName << "\"\n"
                    << "Template ... \"" << dlcustom[mode].modeStr << "\"\n";
        operations  << "[0]\tReturn\n"
                    << "[1]\tReset to default\n"
                    << "[2]\tDelete this mode\n"
                    << "[3]\tMode name .......................... \"" << dlcustom[mode].modeName << "\"\n";
        int index = generateOperation(4, modeMap, dlcustom[mode], operations);
        int operation = Editor::UI::menu(0, index, description.str(), operations.str());
        if(operation < 4) {
            switch(operation) {
                case 0: return;
                case 1: restoreDefault(dlcustom[mode]);                                 break;
                case 2: if(deleteCustom(mode)) return;                                  break;
                case 3: editStr(dlcustom[mode].modeName, "Mode name", "new mode name"); break;
                default: break;
            }
        } else {
            operationSelector(modeMap[operation], dlcustom[mode]);
        }
    }
}
void createCustom() {
    Config temp;
    std::stringstream description, operation, description2, description3;
    description << "Select a template\n";
    operation   << "[0]\tReturn\n"
                << "[1]\ta mode\n"
                << "[2]\tv mode\n"
                << "[3]\tt mode\n"
                << "[4]\tfa mode\n"
                << "[5]\tfv mode\n"
                << "[6]\tft mode\n";
    int mode = Editor::UI::menu(0, 6, description.str(), operation.str());
    if(mode == 0) return;
    temp.initializer(mode);
    description2 << "Template ... \"" << temp.modeStr << "\"\n";
    while(true) {
        std::string tempName = Editor::UI::str(description2.str(), "name for custom mode (without space)");
        if(tempName == "") { Editor::UI::alert("Please enter a mode name!"); continue; }
        if(ConfigCheck::searchCustom(tempName) < 0) { temp.modeName = tempName; break; }
        Editor::UI::alert("Mode name exist!\nPlease try enter again!");
    }
    dlcustom.emplace_back(temp);
    description3 << "Custom mode \"" << temp.modeName << "\" created!";
    Editor::UI::alert(description3.str());
}
bool deleteCustom(int mode) {
    std::string tempName = dlcustom[mode].modeName;
    std::stringstream description, alert;
    description << "Delete custom mode \"" << tempName << "\" ?\n";
    if(Editor::UI::yesNo(description.str())) {
        dlcustom.erase(dlcustom.begin() + mode);
        alert << "Custom mode \"" << tempName << "\" deleted!";
        Editor::UI::alert(alert.str());
        return true;
    }
    return false;
}
void restoreDefault(Config& config) {
    if(Editor::UI::yesNo("Reset to default?\n")) {
        int tempModeInt = config.modeInt;
        std::string tempName = config.modeName;
        config.resetter();
        config.initializer(tempModeInt);
        config.modeName = tempName;
        Editor::UI::alert("Reset succeed!");
    }
}
int generateOperation(int index, std::unordered_map<int, int>& modeMap, Config& config, std::stringstream& operations) {
    bool checkTemplate[6][10] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, //a
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, //v
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0}, //t
        {1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //fa
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //fv
        {0, 0, 1, 1, 0, 0, 0, 1, 1, 1}  //ft
    };
    for(int i=0;i<10;i++) {
        if(checkTemplate[config.modeInt-1][i]) {
            switch(i) {
                case 0: operations  << "[" << index << "]\tKeep original file after convert ... " << (config.keepOriginal ? "YES" : "NO") << "\n";                          break;
                case 1: operations  << "[" << index << "]\tSkip FFmpeg convert ................ " << (config.doNotConvert ? "YES" : "NO") << "\n";                          break;
                case 2: operations  << "[" << index << "]\tCustomize output file name ......... " << (config.customFileName ? "YES" : "NO") << "\n";                        break;
                case 3: operations  << "[" << index << "]\tPrint executed command ............. " << (config.printCommand ? "YES" : "NO") << "\n";                          break;
                case 4: operations  << "[" << index << "]\tOutput file format ................. " << "\"" << config.outputFormat << "\"\n";                                 break;
                case 5: operations  << "[" << index << "]\tOutput audio codec ................. " << "\"" << config.audioCodec << "\"\n";                                   break;
                case 6: operations  << "[" << index << "]\tOutput video codec ................. " << "\"" << config.videoCodec << "\"\n";                                   break;
                case 7: operations  << "[" << index << "]\tAdd index before file name ......... " << (config.addIndex ? "YES" : "NO") << "\n";                              break;
                case 8: operations  << "[" << index << "]\tDefault txt for URL ................ " << (config.isTxtURLSet ? "\""+config.txtURL+"\"" : "NOT SET") << "\n";    break;
                case 9: operations  << "[" << index << "]\tDefault txt for file name .......... " << (config.isTxtNameSet ? "\""+config.txtName+"\"" : "NOT SET") << "\n";  break;
                default: break;
            }
            modeMap[index] = i; index++;
        }
    }
    return index-1;
}
void operationSelector(int operation, Config& config) {
    switch(operation) {
        case 0: editBool(config.keepOriginal, "Keep original file after convert");          break;
        case 1: editBool(config.doNotConvert, "Skip FFmpeg convert");                       break;
        case 2: editBool(config.customFileName, "Customize output file name");              break;
        case 3: editBool(config.printCommand, "Print executed command");                    break;
        case 4: editStr(config.outputFormat, "Output file format", "new output format");    break;
        case 5: editStr(config.audioCodec, "Output audio codec", "new audio codec");        break;
        case 6: editStr(config.videoCodec, "Output video codec", "new video codec");        break;
        case 7: editBool(config.addIndex, "Add index before file name");                    break;
        case 8: editTxtURL(config, "Default txt for URL", "new txt name");                  break;
        case 9: editTxtName(config, "Default txt for file name", "new txt name");           break;
        default: break;
    }
}
void editBool(bool& item, const std::string& str) {
    std::stringstream description;
    description << str << " ... " << (item ? "YES" : "NO") << "\n";
    item = Editor::UI::yesNo(description.str());
}
void editStr(std::string& item, const std::string& str1, const std::string& str2) {
    std::stringstream description;
    description << str1 << " ... " << "\"" << item << "\"\n";
    if(Editor::UI::yesNo(description.str()))
        item = Editor::UI::str(description.str(), str2);
}
void editTxtURL(Config& config, const std::string& str1, const std::string& str2) {
    std::stringstream description;
    description << str1 << " ... " << "\"" << config.txtURL << "\"\n";
    if(Editor::UI::yesNo(description.str()))
        config.setTxtURL(Editor::UI::line(description.str(), str2));
}
void editTxtName(Config& config, const std::string& str1, const std::string& str2) {
    std::stringstream description;
    description << str1 << " ... " << "\"" << config.txtName << "\"\n";
    if(Editor::UI::yesNo(description.str()))
        config.setTxtName(Editor::UI::line(description.str(), str2));
}
}

}
