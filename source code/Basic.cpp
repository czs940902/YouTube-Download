#include "Basic.hpp"

namespace Basic {

namespace Input {
bool getInt(int& n) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    int value; char extra;
    if(ss >> value && !(ss >> extra)) {
        n = value; return true;
    }
    return false;
}
bool getWord(std::string& str) {
    std::string input, text;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    ss >> text; str = text;
    return true;
}
bool getLine(std::string& str) {
    std::string input;
    std::getline(std::cin, input);
    str = input;
    return true;
}
}

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
        for(Config& config : dlcustom)
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
int input(int argc, const char *argv[], Config& config) {
    int status = mode(argc, argv, config);
    if(status == 0) return 0;
    if(argc > 2)
        if(!arg(argc, argv, config))
            return 0;
    return status;
}
int mode(int argc, const char *argv[], Config& config) {
    std::unordered_map<std::string, int> modeMap =
    { {"a", 1}, {"v", 2}, {"t", 3}, {"fa", 4}, {"fv", 5}, {"ft", 6}, {"u", 7}, {"edit", 8} };
    int mode = modeMap[std::string(argv[1])];
    switch(mode) {
        case 0: {
            int indexCustom = ConfigCheck::searchCustom(std::string(argv[1]));
            if(indexCustom < 0) {
                std::cout << "Mode does not exist!\n\n";
                Basic::Information::printModes();
                Basic::Information::printCustom();
                return 0;
            }
            config = dlcustom[indexCustom];
            std::cout << "[Custom mode] \"" << config.modeName << "\"\n";
            return 1;
        }
        case 1: case 2: case 3: case 4: case 5: case 6:
            config = dlconfig[mode];
            return 1;
        case 7:
            config.modeInt = 7;
            return 2;
        case 8:
            config.modeInt = 8;
            return 3;
        default:
            return 0;
        //return value { 0_error, 1_task, 2_update, 3_edit }
    }
}
bool arg(int argc, const char *argv[], Config& config) {
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
    for(int i=2;i<argc;i++) {
        int arg = argMap[std::string(argv[i])];
        if(!checkMap[config.modeInt][arg]) {
            std::cout << "Argument is not avaliable in mode \"" << config.modeStr << "\": \"" << argv[i] << "\"\n\n";
            Basic::Information::printArguments();
            return false;
        }
        switch(arg) {
            case 0:
                std::cout << "Argument does not exist: \"" << argv[i] << "\"\n\n";
                Basic::Information::printArguments();
                return false;
            case 1:
                config.keepOriginal = true; isSetK = true;
                arguments << "\"" << argv[i] << "\" ";
                break;
            case 2:
                config.doNotConvert = true;
                arguments << "\"" << argv[i] << "\" ";
                break;
            case 3:
                config.customFileName = true;
                arguments << "\"" << argv[i] << "\" ";
                break;
            case 4:
                config.printCommand = true;
                arguments << "\"" << argv[i] << "\" ";
                break;
            case 5:
                if(i+1 == argc || argv[i+1][0] == '-') {
                    std::cout << "Argument \"-f\" without file format\n"
                              << "Example: \"dl a -f m4a\", \"dl v -f mp4\"\n\n";
                    return false;
                }
                config.outputFormat = argv[i+1]; isSetF = true;
                arguments << "\"" << argv[i] << " " << argv[++i] << "\" ";
                break;
            case 6:
                config.addIndex = true;
                arguments << "\"" << argv[i] << "\" ";
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
