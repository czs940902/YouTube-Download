#include "Basic.hpp"
#include "Tool.hpp"
#include "Main.hpp"
#include "Config.hpp"
#include "Updater.hpp"
#include "Editor.hpp"
#include "YouTube.hpp"

int main(int argc, const char *argv[]) {
    Config0 config;
    Config::RW::initialize();
    if(argc == 1) { Main::Information::printInfo(); return 0; }
    switch(Main::Parse::input(argc, argv, config)) {
        case 0: return 1;
        case 1: { YouTube task(config); }           break;
        case 2: Tool::update(config.printCommand);  break;
        case 3: Editor::editor();                   break;
        default: return 0;
    }
}
