#include "Main.hpp"

namespace Main {

namespace Information {
void printInfo() {
    std::cout   << "YouTube Download Assistant (by czs940902)\n"
                << "Version " << VERSION << " " << Tool::getTargetOS() << " (" << BUILDDATE << ")\n\n";
    printModes(); printCustom(); printArguments();
    std::cout   << "Example: \"dl t\", \"dl a -k\", \"dl v -f mp4\", \"dl edit\"\n\n";
}
void printCustom() {
    if(dlcustom.size() > 0) {
        std::cout   << "[Custom modes]\n  ";
        for(Config0& config : dlcustom)
            std::cout << "\"" << config.modeName << "\" ";
        std::cout   << "\n\n";
    }
}
void printModes() {
    std::cout   << "[Modes]\n"
                << "  [a]    Download audio\n"
                << "  [v]    Download video\n"
                << "  [t]    Download thumbnail\n"
                << "  [fa]   Download audios from txt file\n"
                << "  [fv]   Download videos from txt file\n"
                << "  [ft]   Download thumbnails from txt file\n"
                << "  [u]    Update yt-dlp & ffmpeg\n"
                << "  [edit] Edit config and favorites\n"
                << "\n";
}
void printArguments() {
    std::cout   << "[Arguments]\n"
                << "  [-k]   Keep original file after FFmpeg convert  (a, v, fa, fv)\n"
                << "  [-n]   Skip FFmpeg convert                      (a, v, fa, fv)\n"
                << "  [-c]   Customize output file name               (a, v, t, fa, fv, ft)\n"
                << "  [-p]   Print executed commands                  (a, v, t, fa, fv, ft, u)\n"
                << "  [-f]   Specify output format                    (a, v, fa, fv)\n"
                << "  [-i]   Add index before file name               (fa, fv, ft)\n"
                << "\n";
}
}

namespace Parse {
int input(int argc, const char *argv[]) {
    Config::RW::initialize();
    std::string modeStr;
    std::vector<std::string> args;
    if(argc > 1) {
        modeStr = argv[1];
        for(int i=2;i<argc;i++) args.emplace_back(argv[i]);
        return run(modeStr, args);
    } else {
        int result = 0;
        std::string input, temp;
        Main::Information::printInfo();
        std::cout << "Command > dl ";
        Basic::Input::getLine(input);
        if(input != "") {
            std::stringstream ss(input);
            ss >> modeStr;
            while(ss >> temp) args.emplace_back(temp);
            result = run(modeStr, args);
        }
        std::cout << "Press enter to exit...";
        Basic::Input::getEnter();
        return result;
    }
}
int run(std::string& modeStr, std::vector<std::string>& args) {
    Config0 config;
    if(!mode(modeStr, config))  return 1;
    if(!arg(args, config))      return 1;
    YouTube task(config);
    int status = task.run();
    switch(status) {
        case 0:                                     return 0;
        case 1:                                     return 1;
        case 2: Tool::update(config.printCommand);  return 0;
        case 3: Editor::editor();                   return 0;
        default:                                    return 1;
    }
}
bool mode(std::string& modeStr, Config0& config) {
    std::unordered_map<std::string, int> modeMap =
    { {"a", 1}, {"v", 2}, {"t", 3}, {"fa", 4}, {"fv", 5}, {"ft", 6}, {"u", 7}, {"edit", 8} };
    int mode = modeMap[modeStr];
    switch(mode) {
        case 0: {
            int indexCustom = Config::Check::searchCustom(modeStr);
            if(indexCustom < 0) {
                std::cout << "Mode does not exist!\n\n";
                Main::Information::printModes();
                Main::Information::printCustom();
                return false;
            }
            config = dlcustom[indexCustom];
            std::cout << "[Custom mode] \"" << config.modeName << "\"\n";
            return true;
        }
        case 1: case 2: case 3: case 4: case 5: case 6:
            config = dlconfig[mode];
            return true;
        default:
            config.modeInt = mode;
            return true;
    }
}
bool arg(std::vector<std::string>& args, Config0& config) {
    if(args.size() == 0) return true;
    const bool checkMap[9][7] = {
        {1, 0, 0, 0, 0, 0, 0}, // modeInt = 0
        {1, 1, 1, 1, 1, 1, 0}, // modeInt = 1
        {1, 1, 1, 1, 1, 1, 0}, // modeInt = 2
        {1, 0, 0, 1, 1, 0, 0}, // modeInt = 3
        {1, 1, 1, 1, 1, 1, 1}, // modeInt = 4
        {1, 1, 1, 1, 1, 1, 1}, // modeInt = 5
        {1, 0, 0, 1, 1, 0, 1}, // modeInt = 6
        {1, 0, 0, 0, 1, 0, 0}, // modeInt = 7
        {1, 0, 0, 0, 0, 0, 0}  // modeInt = 8
    };
    bool isSetK = false, isSetF = false;
    std::stringstream arguments;
    arguments << "[Arguments] ";
    std::unordered_map<std::string, int> argMap =
    { {"-k", 1}, {"-n", 2}, {"-c", 3}, {"-p", 4}, {"-f", 5}, {"-i", 6} };
    for(int i=0;i<args.size();i++) {
        int arg = argMap[std::string(args[i])];
        if(!checkMap[config.modeInt][arg]) {
            std::cout << "Argument is not avaliable in mode \"" << config.modeStr << "\": \"" << args[i] << "\"\n\n";
            Main::Information::printArguments();
            return false;
        }
        switch(arg) {
            case 0:
                std::cout << "Argument does not exist: \"" << args[i] << "\"\n\n";
                Main::Information::printArguments();
                return false;
            case 1:
                config.keepOriginal = true; isSetK = true;
                arguments << "\"" << args[i] << "\" ";
                break;
            case 2:
                config.doNotConvert = true;
                arguments << "\"" << args[i] << "\" ";
                break;
            case 3:
                config.customFileName = true;
                arguments << "\"" << args[i] << "\" ";
                break;
            case 4:
                config.printCommand = true;
                arguments << "\"" << args[i] << "\" ";
                break;
            case 5:
                if(i+1 == args.size() || args[i+1][0] == '-') {
                    std::cout << "Argument \"-f\" without file format\n"
                              << "Example: \"dl a -f m4a\", \"dl v -f mp4\"\n\n";
                    return false;
                }
                config.outputFormat = args[i+1]; isSetF = true;
                arguments << "\"" << args[i] << " " << args[++i] << "\" ";
                break;
            case 6:
                config.addIndex = true;
                arguments << "\"" << args[i] << "\" ";
                break;
            default: break;
        }
    }
    if(config.doNotConvert && (isSetK || isSetF)) {
        std::cout << "Argument \"-k\" and \"-f\" is not avaliable when \"-n\" is used\n\n";
        return false;
    }
    std::cout << arguments.str() << "\n\n";
    return true;
}
}

}
