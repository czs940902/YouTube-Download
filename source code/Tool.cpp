#include "Tool.hpp"

#if defined(MACOS)

namespace Tool {
void cleanScreen() {
    system("clear");
}
std::string getHomePath() {
    const char *homePath = std::getenv("HOME");
    if(homePath) return std::string(homePath);
    return "";
}
std::string getTargetOS() {
    return "macOS Universal";
}
bool runCommand(const std::string& command, bool printCommand) {
    if(printCommand) std::cout << "[Command] " << command << "\n\n";
    int status = system(command.c_str()); std::cout << "\n";
    if(status) return false; return true;
}
bool downloadJPG(const std::string url, const std::string fileName, bool printCommand) {
    std::string command = "curl -s \"" + url + "\" -o \"" + fileName + ".jpg\"";
    if(runCommand(command, printCommand)) return true; return false;
}
void update(bool printCommand) {
    std::cout << "[Update yt-dlp & ffmpeg]\n\n";
    std::cout << "Updating yt-dlp...\n";
    runCommand("brew upgrade yt-dlp", printCommand);
    std::cout << "Updating FFmpeg...\n";
    runCommand("brew upgrade ffmpeg", printCommand);
    std::cout << "Update finished!\n\n";
}
}

#endif



#if defined(WINX64)

namespace Tool {
void cleanScreen() {
    system("cls");
}
std::string getHomePath() {
    const char *homePath = std::getenv("USERPROFILE");
    if(homePath) return std::string(homePath);
    return "";
}
std::string getTargetOS() {
    return "Windows x86_64";
}
bool runCommand(const std::string& command, bool printCommand) {
    if(printCommand) std::cout << "[Command] " << command << "\n\n";
    int status = system(command.c_str()); std::cout << "\n";
    if(status) return false; return true;
}
bool downloadJPG(const std::string url, const std::string fileName, bool printCommand) {
    std::string command = "curl -s \"" + url + "\" -o \"" + fileName + ".jpg\"";
    if(runCommand(command, printCommand)) return true; return false;
}
void update(bool printCommand) {
    std::cout << "[Update yt-dlp & ffmpeg]\n\n";
    std::cout << "Updating yt-dlp...\n";
    runCommand("yt-dlp -U", printCommand);
    std::cout << "Please update FFmpeg manually!\n\n";
    std::cout << "Update finished!\n\n";
}
}

#endif



#if defined(WINA64)

namespace Tool {
void cleanScreen() {
    system("cls");
}
std::string getHomePath() {
    const char *homePath = std::getenv("USERPROFILE");
    if(homePath) return std::string(homePath);
    return "";
}
std::string getTargetOS() {
    return "Windows ARM64";
}
bool runCommand(const std::string& command, bool printCommand) {
    if(printCommand) std::cout << "[Command] " << command << "\n\n";
    int status = system(command.c_str()); std::cout << "\n";
    if(status) return false; return true;
}
bool downloadJPG(const std::string url, const std::string fileName, bool printCommand) {
    std::string command = "curl -s \"" + url + "\" -o \"" + fileName + ".jpg\"";
    if(runCommand(command, printCommand)) return true; return false;
}
void update(bool printCommand) {
    std::cout << "[Update yt-dlp & ffmpeg]\n\n";
    std::cout << "Updating yt-dlp...\n";
    runCommand("yt-dlp -U", printCommand);
    std::cout << "Please update FFmpeg manually!\n\n";
    std::cout << "Update finished!\n\n";
}
}

#endif
