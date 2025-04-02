#ifndef YouTube_hpp
#define YouTube_hpp

#include "Basic.hpp"
#include "Tool.hpp"
#include "Config.hpp"

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <regex>

class YouTube {
public:
    YouTube(const Config& temp);
private:
    Config config;
    int countDownload       = 0;
    int countSucceed        = 0;
    int countFailed         = 0;
    std::string videoID     = "";
    std::string inputURL    = "";
    std::string standardURL = "";
    std::string fileName    = "";
    std::string audioName   = "";
    std::string videoName   = "";
    std::ifstream txtLog;
    std::ifstream txtURL;
    std::ifstream txtName;
    
    void modeA();
    void modeV();
    void modeT();
    void modeFA();
    void modeFV();
    void modeFT();
    
    bool getURLFromInput();
    bool getNameFromInput();
    bool setSourceFile();
    bool getInputFromFile();
    bool parseVideoID();
    bool setFileName();
    
    bool getAudioFile();
    bool getVideoFile();
    bool getThumbnailFile();
    bool convertAudio();
    bool convertVideo();
};

#endif
