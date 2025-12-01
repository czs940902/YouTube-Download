#include "YouTube.hpp"

YouTube::YouTube(const Config0& temp) {
    config = temp;
}

int YouTube::run() {
    switch(config.modeInt) {
        case 1: return modeA();
        case 2: return modeV();
        case 3: return modeT();
        case 4: return modeFA();
        case 5: return modeFV();
        case 6: return modeFT();
        case 7: return 2;
        case 8: return 3;
        default: return 1;
    }
}

int YouTube::modeA() {
    std::cout << "[Download audio]\n";
    if(!getURLFromInput())  return 1;
    if(!getNameFromInput()) return 1;
    if(!setFileName())      return 1;
    if(!getAudioFile())     return 1;
    if(!convertAudio())     return 1;
    std::cout << "Download completed!\n\n";
    return 0;
}
int YouTube::modeV() {
    std::cout << "[Download video]\n";
    if(!getURLFromInput())  return 1;
    if(!getNameFromInput()) return 1;
    if(!setFileName())      return 1;
    if(!getAudioFile())     return 1;
    if(!getVideoFile())     return 1;
    if(!convertVideo())     return 1;
    std::cout << "Download completed!\n\n";
    return 0;
}
int YouTube::modeT() {
    std::cout << "[Download thumbnail]\n";
    if(!getURLFromInput())  return 1;
    if(!getNameFromInput()) return 1;
    if(!setFileName())      return 1;
    if(!getThumbnailFile()) return 1;
    std::cout << "Download completed!\n\n";
    return 0;
}
int YouTube::modeFA() {
    std::cout << "[Download audios from txt file]\n";
    if(!setSourceFile())    return 1;
    while(getInputFromFile()) {
        countDownload++;
        std::cout << "Downloading audio " << countDownload << "...\n\n";
        if(!parseVideoID()) { countFailed++; std::cout << "Audio " << countDownload << " download failed!\n\n"; continue; }
        if(!setFileName())  { countFailed++; std::cout << "Audio " << countDownload << " download failed!\n\n"; continue; }
        if(!getAudioFile()) { countFailed++; std::cout << "Audio " << countDownload << " download failed!\n\n"; continue; }
        if(!convertAudio()) { countFailed++; std::cout << "Audio " << countDownload << " download failed!\n\n"; continue; }
        countSucceed++; std::cout << "Audio " << countDownload << " download succeed!\n\n";
    }
    std::cout << "Download completed!\n"
              << countDownload << " total\n"
              << countSucceed << " succeed\n"
              << countFailed << " failed\n\n";
    return 0;
}
int YouTube::modeFV() {
    std::cout << "[Download videos from txt file]\n";
    if(!setSourceFile())    return 1;
    while(getInputFromFile()) {
        countDownload++;
        std::cout << "Downloading video " << countDownload << "...\n\n";
        if(!parseVideoID()) { countFailed++; std::cout << "Video " << countDownload << " download failed!\n\n"; continue; }
        if(!setFileName())  { countFailed++; std::cout << "Video " << countDownload << " download failed!\n\n"; continue; }
        if(!getAudioFile()) { countFailed++; std::cout << "Video " << countDownload << " download failed!\n\n"; continue; }
        if(!getVideoFile()) { countFailed++; std::cout << "Video " << countDownload << " download failed!\n\n"; continue; }
        if(!convertVideo()) { countFailed++; std::cout << "Video " << countDownload << " download failed!\n\n"; continue; }
        countSucceed++; std::cout << "Video " << countDownload << " download succeed!\n\n";
    }
    std::cout << "Download completed!\n"
              << countDownload << " total\n"
              << countSucceed << " succeed\n"
              << countFailed << " failed\n\n";
    return 0;
}
int YouTube::modeFT() {
    std::cout << "[Download thumbnails from txt file]\n";
    if(!setSourceFile())    return 1;
    while(getInputFromFile()) {
        countDownload++;
        std::cout << "Downloading thumbnail " << countDownload << "...\n\n";
        if(!parseVideoID())     { countFailed++; std::cout << "Thumbnail " << countDownload << " download failed!\n\n"; continue; }
        if(!setFileName())      { countFailed++; std::cout << "Thumbnail " << countDownload << " download failed!\n\n"; continue; }
        if(!getThumbnailFile()) { countFailed++; std::cout << "Thumbnail " << countDownload << " download failed!\n\n"; continue; }
        countSucceed++; std::cout << "Thumbnail " << countDownload << " download succeed!\n\n";
    }
    std::cout << "Download completed!\n"
              << countDownload << " total\n"
              << countSucceed << " succeed\n"
              << countFailed << " failed\n\n";
    return 0;
}

bool YouTube::getURLFromInput() {
    std::cout << "Enter YouTube URL: ";
    Basic::Input::getLine(inputURL);
    if(parseVideoID()) {
        std::cout << "Download from URL: " << standardURL << "\n\n";
        return true;
    } else {
        std::cout << "Invalid YouTube URL!\n\n";
        return false;
    }
}
bool YouTube::getNameFromInput() {
    if(!config.customFileName) return true;
    std::cout << "Enter custom file name: ";
    Basic::Input::getLine(fileName);
    if(fileName == "") {
        std::cout << "Please enter a file name!\n\n";
        return false;
    } else {
        std::cout << "Using custom file name: " << fileName << "\n\n";
        return true;
    }
}
bool YouTube::setSourceFile() {
    std::string inputTxtURL, inputTxtName;
    std::cout << "Enter txt file including YouTube URLs: ";
    Basic::Input::getLine(inputTxtURL);
    if(inputTxtURL == "") {
        if(!config.isTxtURLSet) { std::cout << "Please enter a file name!\n\n"; return false; }
        txtURL.open(config.txtURL);
        if(!txtURL.is_open()) { std::cout << "Please enter a file name!\n\n"; return false; }
        std::cout << "Read YouTube URLs from given txt file: " << config.txtURL << " (default)\n\n";
    } else {
        txtURL.open(inputTxtURL);
        if(!txtURL.is_open()) { std::cout << "File does not exist!\n\n"; return false; }
        std::cout << "Read YouTube URLs from given txt file: " << inputTxtURL << "\n\n";
    }
    if(!config.customFileName) return true;
    std::cout << "Enter txt file including custom file names: ";
    Basic::Input::getLine(inputTxtName);
    if(inputTxtName == "") {
        if(!config.isTxtNameSet) { std::cout << "Please enter a file name!\n\n"; return false; }
        txtName.open(config.txtName);
        if(!txtName.is_open()) { std::cout << "Please enter a file name!\n\n"; return false; }
        std::cout << "Read custom file names from given txt file: " << config.txtName << " (default)\n\n";
    } else {
        txtName.open(inputTxtName);
        if(!txtName.is_open()) { std::cout << "File does not exist!\n\n"; return false; }
        std::cout << "Read custom file names from given txt file: " << inputTxtName << "\n\n";
    }
    return true;
}
bool YouTube::getInputFromFile() {
    inputURL = ""; fileName = "";
    if(!std::getline(txtURL, inputURL)) { txtURL.close(); txtName.close(); return false; }
    if(!config.customFileName) return true;
    if(!std::getline(txtName, fileName)) { txtURL.close(); txtName.close(); return false;}
    return true;
}
bool YouTube::parseVideoID() {
    std::smatch match;
    std::regex youtubeRegex(R"((?:https?:\/\/)?(?:www\.|m\.)?(?:youtube\.com\/(?:watch\?v=|live\/|shorts\/)|youtu\.be\/)?([a-zA-Z0-9_-]{11}))");
    if(std::regex_search(inputURL, match, youtubeRegex)) {
        videoID = match[1].str();
        standardURL = "https://youtu.be/" + videoID;
        return true;
    } else if(inputURL.length() == 11 && std::regex_match(inputURL, std::regex("^[a-zA-Z0-9_-]{11}$"))) {
        videoID = inputURL;
        standardURL = "https://youtu.be/" + videoID;
        return true;
    } else {
        return false;
    }
}
bool YouTube::setFileName() {
    if(!config.customFileName) fileName = videoID;
    if(config.addIndex) fileName = std::to_string(countDownload) + "_" + fileName;
    return true;
}

bool YouTube::getAudioFile() {
    audioName = "";
    std::string urlCommand = "-f bestaudio \"" + standardURL + "\"";
    std::string nameCommand = "-o \"" + fileName + "_a.%(ext)s\"";
    std::string logCommand = "--print-to-file after_move:filename dlAudioNameLog.txt";
    std::string command = "yt-dlp " + urlCommand + " " + nameCommand + " " + logCommand;
    std::cout << "Downloading audio file...\n";
    if(!Tool::runCommand(command, config.printCommand)) { std::cout << "Download failed!\n\n"; return false; }
    txtLog.open("dlAudioNameLog.txt");
    if(!txtLog.is_open()) { std::cout << "Get Audio Name failed!\n\n"; return false; }
    std::getline(txtLog, audioName);
    txtLog.close();
    std::remove("dlAudioNameLog.txt");
    return true;
}
bool YouTube::getVideoFile() {
    videoName = "";
    std::string urlCommand = "-f bestvideo \"" + standardURL + "\"";
    std::string nameCommand = "-o \"" + fileName + "_v.%(ext)s\"";
    std::string logCommand = "--print-to-file after_move:filename dlVideoNameLog.txt";
    std::string command = "yt-dlp " + urlCommand + " " + nameCommand + " " + logCommand;
    std::cout << "Downloading video file...\n";
    if(!Tool::runCommand(command, config.printCommand)) { std::cout << "Download failed!\n\n"; return false; }
    txtLog.open("dlVideoNameLog.txt");
    if(!txtLog.is_open()) { std::cout << "Get Video Name failed!\n\n"; return false; }
    std::getline(txtLog, videoName);
    txtLog.close();
    std::remove("dlVideoNameLog.txt");
    return true;
}
bool YouTube::getThumbnailFile() {
    std::string url = "http://img.youtube.com/vi/" + videoID + "/maxresdefault.jpg";
    std::cout << "Downloading thumbnail file...\n";
    if(!Tool::downloadJPG(url, fileName, config.printCommand)) {
        std::cout << "Download failed!\n\n";
        return false;
    }
    return true;
}
bool YouTube::convertAudio() {
    if(config.doNotConvert) return true;
    std::cout << "[Audio file name] " << audioName << "\n\n";
    std::string audioFile = "-i \"" + audioName + "\"";
    std::string outputFile = "\"" + fileName + "." + config.outputFormat + "\"";
    std::string command = "ffmpeg " + audioFile + " " + outputFile;
    std::cout << "Converting with FFmpeg...\n";
    if(!Tool::runCommand(command, config.printCommand)) {
        std::cout << "Convert failed!\n\n";
        return false;
    }
    if(!config.keepOriginal) {
        std::cout << "Deleteing audio file \"" << audioName << "\"\n\n";
        std::remove(audioName.c_str());
    }
    return true;
}
bool YouTube::convertVideo() {
    if(config.doNotConvert) return true;
    std::cout << "[Audio file name] " << audioName << "\n"
              << "[Video file name] " << videoName << "\n\n";
    std::string audioFile = "-i \"" + audioName + "\"";
    std::string videoFile = "-i \"" + videoName + "\"";
    std::string audioCodec = "-c:a " + config.audioCodec;
    std::string videoCodec = "-c:v " + config.videoCodec;
    std::string outputFile = "\"" + fileName + "." + config.outputFormat + "\"";
    std::string command = "ffmpeg " + audioFile + " " + videoFile + " " + audioCodec + " " + videoCodec + " " + outputFile;
    std::cout << "Converting with FFmpeg...\n";
    if(!Tool::runCommand(command, config.printCommand)) {
        std::cout << "Convert failed!\n\n";
        return false;
    }
    if(!config.keepOriginal) {
        std::cout << "Deleteing audio file \"" << audioName << "\"\n";
        std::remove(audioName.c_str());
        std::cout << "Deleteing video file \"" << videoName << "\"\n\n";
        std::remove(videoName.c_str());
    }
    return true;
}
