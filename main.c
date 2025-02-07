#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER      "3.1"
#define DATE     "20250207"

#define LEN_URL  100
#define LEN_VID  20
#define LEN_FILE 200
#define LEN_CMD  500

//參數旗標
char mode               = 0;
char flagKeepOriginal   = 0;
char flagNoConvert      = 0;
char flagAddIndex       = 0;
char flagCustomFileName = 0;
char flagConvertM4A     = 0;
char flagPrintCommand   = 0;

//取決於作業系統
#if defined(__APPLE__) && defined(__MACH__)
#define OS "macOS" //macOS專區
int runCommand(char *command) {
    if(flagPrintCommand) printf("[指令] %s\n", command);
    int status = system(command); puts("");
    if(status) return 1;
    return 0;
    //成功_0, 失敗_1
}
int getCommandResult(char *command, char *result) {
    if(flagPrintCommand) printf("[指令] %s\n", command);
    FILE *fp = popen(command, "r"); puts("");
    if(!fp) return 1;
    if(!fgets(result, LEN_FILE+5, fp)) { pclose(fp); return 1; }
    result[strcspn(result, "\n")] = 0;
    pclose(fp);
    return 0;
    //成功_0, 失敗_1
}
int downloadFile(char *filename, char *url) {
    char command[LEN_CMD];
    sprintf(command, "curl -s \"%s\" -o \"%s.jpg\"", url, filename);
    if(runCommand(command)) return 1;
    return 0;
    //成功_0, 失敗_1
}
int updateYTDLP(void) {
    puts("正在更新yt-dlp...");
    if(runCommand("brew upgrade yt-dlp")) return 1;
    return 0;
    //成功_0, 失敗_1
}
int updateFFmpeg(void) {
    puts("正在更新ffmpeg...");
    if(runCommand("brew upgrade ffmpeg")) return 1;
    return 0;
    //成功_0, 失敗_1
}
#elif defined(_WIN32)
#define OS "Windows" //Windows專區
int runCommand(char *command) {
    if(flagPrintCommand) printf("[指令] %s\n", command);
    int status = system(command); puts("");
    if(status) return 1;
    return 0;
    //成功_0, 失敗_1
}
int getCommandResult(char *command, char *result) {
    if(flagPrintCommand) printf("[指令] %s\n", command);
    FILE *fp = _popen(command, "r"); puts("");
    if(!fp) return 1;
    if(!fgets(result, LEN_FILE+5, fp)) { _pclose(fp); return 1; }
    result[strcspn(result, "\n")] = 0;
    _pclose(fp);
    return 0;
    //成功_0, 失敗_1
}
int downloadFile(char *filename, char *url) {
    char command[LEN_CMD];
    sprintf(command, "curl -s \"%s\" -o \"%s.jpg\"", url, filename);
    if(runCommand(command)) return 1;
    return 0;
    //成功_0, 失敗_1
}
int updateYTDLP(void) {
    puts("正在更新yt-dlp...");
    if(runCommand("yt-dlp -U")) return 1;
    return 0;
    //成功_0, 失敗_1
}
int updateFFmpeg(void) {
    puts("請手動更新ffmpeg！\n");
    return 0;
    //成功_0
}
#endif

//輸出提示資訊、處理命令列參數
void printAvailableMode(void) {
    puts("【可用模式】");
    puts("[u]  更新 yt-dlp 與 ffmpeg");
    puts("[a]  音訊下載");
    puts("[v]  影片下載");
    puts("[t]  縮圖下載");
    puts("[fa] 讀檔下載(音訊)");
    puts("[fv] 讀檔下載(影片)");
    puts("[ft] 讀檔下載(縮圖)");
    puts("");
}
void printAvailableArguments(void) {
    puts("【可用參數】");
    puts("[-k]   在轉檔後保留原始下載檔案\t(在 a, v, fa, fv 模式可用)");
    puts("[-n]   跳過 ffmpeg 轉檔\t\t(在 a, v, fa, fv 模式可用)");
    puts("[-c]   自訂檔案名稱\t\t(在 a, v, t, fa, fv, ft 模式可用)");
    puts("[-i]   在檔名加入索引值\t\t(在 fa, fv, ft 模式可用)");
    puts("[-m4a] 轉檔為 m4a 格式\t\t(在 a, fa 模式可用)");
    puts("[-p]   輸出執行的指令\t\t(在所有模式可用)");
    puts("");
}
int parseArguments(int argc, char *argv[]) {
    char checkArg[7][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0}, //0
        {0, 0, 1, 1, 0, 1, 1, 0}, //arg 1 -k
        {0, 0, 1, 1, 0, 1, 1, 0}, //arg 2 -n
        {0, 0, 1, 1, 1, 1, 1, 1}, //arg 3 -c
        {0, 0, 0, 0, 0, 1, 1, 1}, //arg 4 -i
        {0, 0, 1, 0, 0, 1, 0, 0}, //arg 5 -m4a
        {0, 1, 1, 1, 1, 1, 1, 1}  //arg 6 -p
        //{0, U, A, V, T, FA, FV ,FT}
    };
    if(argc == 1) {
        puts("歡迎使用YouTube下載輔助工具！");
        puts("作者：czs940902");
        printf("版本：Version %s %s (%s)\n\n", VER, OS, DATE);
        printAvailableMode();
        printAvailableArguments();
        puts("範例指令：\"dl a\", \"dl v -k\"\n");
        return 1;
    }
    else if(strcmp(argv[1], "u")  == 0) mode = 1;
    else if(strcmp(argv[1], "a")  == 0) mode = 2;
    else if(strcmp(argv[1], "v")  == 0) mode = 3;
    else if(strcmp(argv[1], "t")  == 0) mode = 4;
    else if(strcmp(argv[1], "fa") == 0) mode = 5;
    else if(strcmp(argv[1], "fv") == 0) mode = 6;
    else if(strcmp(argv[1], "ft") == 0) mode = 7;
    else { puts("模式錯誤！\n"); printAvailableMode(); return 1; }
    if(argc > 2) {
        for(int i=2;i<argc;i++) {
                 if(strcmp(argv[i], "-k")   == 0 && checkArg[1][mode]) { flagKeepOriginal = 1; }
            else if(strcmp(argv[i], "-n")   == 0 && checkArg[2][mode]) { flagNoConvert = 1; flagKeepOriginal = 1; }
            else if(strcmp(argv[i], "-c")   == 0 && checkArg[3][mode]) { flagCustomFileName = 1; }
            else if(strcmp(argv[i], "-i")   == 0 && checkArg[4][mode]) { flagAddIndex = 1; }
            else if(strcmp(argv[i], "-m4a") == 0 && checkArg[5][mode]) { flagConvertM4A = 1; }
            else if(strcmp(argv[i], "-p")   == 0 && checkArg[6][mode]) { flagPrintCommand = 1; }
            else { printf("無效的參數：%s\n\n", argv[i]); printAvailableArguments(); return 1; }
        }
        printf("使用參數："); for(int i=2;i<argc;i++) printf("%s ", argv[i]); puts("\n");
    }
    return 0;
    //成功_0, 失敗_1
}

//YouTube連結處理
int getVideoIDFromURL(char *url, char *id) {
    char *start = NULL;
    if((start = strstr(url, "youtu.be/"))) {
        sscanf(start+9, "%11s", id);
    } else if((start = strstr(url, "watch?v="))) {
        sscanf(start+8, "%11s", id);
    } else if((start = strstr(url, "/live/"))) {
        sscanf(start+6, "%11s", id);
    } else { puts("連結錯誤！\n"); return 1; }
    return 0;
    //成功_0, 失敗_1
}
int getURLFromInput(char *mode, char *id) {
    char buffer[LEN_URL];
    printf("[%s下載模式]\n請輸入YouTube連結：", mode);
    fgets(buffer, LEN_URL, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(getVideoIDFromURL(buffer, id)) return 1;
    printf("將下載YouTube%s：https://youtu.be/%s\n\n", mode, id);
    return 0;
    //成功_0, 失敗_1
}

//自訂檔名輸入
void getCustomFileName(char *filename) {
    printf("請輸入自訂檔案名稱：");
    fgets(filename, LEN_FILE, stdin);
    filename[strcspn(filename, "\n")] = 0;
    printf("將使用自訂檔案名稱：%s\n\n", filename);
}

//讀檔相關
FILE *URL, *NAME;
int setSourceFile(char *mode) {
    char buffer[LEN_FILE];
    printf("[讀檔下載模式(%s)]\n請輸入包含連結的檔案名稱：", mode);
    fgets(buffer, LEN_FILE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strcmp(buffer, "") == 0) {
        URL = fopen("url.txt", "r");
        if(!URL) { puts("檔案錯誤！\n"); return 1; }
        puts("將從此檔案讀取連結：url.txt (預設值)\n");
    } else {
        URL = fopen(buffer, "r");
        if(!URL) { puts("檔案錯誤！\n"); return 1; }
        printf("將從此檔案讀取連結：%s\n\n", buffer);
    }
    if(flagCustomFileName) {
        printf("請輸入包含檔名的檔案名稱：");
        fgets(buffer, LEN_FILE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if(strcmp(buffer, "") == 0) {
            NAME = fopen("name.txt", "r");
            if(!NAME) { puts("檔案錯誤！\n"); return 1; }
            puts("將從此檔案讀取檔名：name.txt (預設值)\n");
        } else {
            NAME = fopen(buffer, "r");
            if(!NAME) { puts("檔案錯誤！\n"); return 1; }
            printf("將從此檔案讀取檔名：%s\n\n", buffer);
        }
    }
    return 0;
    //成功_0, 失敗_1
}
int getDataFromFile(int index, char *id, char *filename) {
    char buffer_url[LEN_URL], buffer_name[LEN_FILE];
    if(!fgets(buffer_url, LEN_URL, URL)) return 0;
    buffer_url[strcspn(buffer_url, "\n")] = 0;
    if(getVideoIDFromURL(buffer_url, id)) return -1;
    if(flagCustomFileName) {
        if(!fgets(buffer_name, LEN_FILE, NAME)) return 0;
        buffer_name[strcspn(buffer_name, "\n")] = 0;
    } else {
        strcpy(buffer_name, id);
    }
    if(flagAddIndex) {
        sprintf(filename, "%d_%s", index, buffer_name);
    } else {
        strcpy(filename, buffer_name);
    }
    return 1;
    //成功_1, 失敗_0, VID無法解析_-1
}

//檢查檔案是否存在
int checkFileExist(char *filename) {
    FILE *file = fopen(filename, "r");
    if(file) { fclose(file); return 0; }
    return 1;
    //成功_0, 失敗_1
}

//yt-dlp相關
int getAudioFile(char *filename, char *id) {
    char file[LEN_FILE+12], url[LEN_URL], command[LEN_CMD];
    sprintf(file, "\"%s_a.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp -f bestaudio -o %s %s", file, url);
    puts("正在下載音訊部分...");
    if(runCommand(command)) { puts("下載失敗！\n"); return 1; }
    return 0;
    //成功_0, 失敗_1
}
int getVideoFile(char *filename, char *id) {
    char file[LEN_FILE+12], url[LEN_URL], command[LEN_CMD];
    sprintf(file, "\"%s_v.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp -f bestvideo -o %s %s", file, url);
    puts("正在下載影片部分...");
    if(runCommand(command)) { puts("下載失敗！\n"); return 1; }
    return 0;
    //成功_0, 失敗_1
}
int getAudioName(char *filename, char *id, char *file_a) {
    char file[LEN_FILE+12], url[LEN_URL], command[LEN_CMD];
    sprintf(file, "\"%s_a.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp --get-filename -f bestaudio -o %s %s", file, url);
    puts("正在取得音訊檔名...");
    if(getCommandResult(command, file_a)) { puts("getAudioName失敗！\n"); return 1; }
    if(!strstr(file_a, filename))         { puts("getAudioName失敗！\n"); return 1; }
    if(checkFileExist(file_a))            { puts("getAudioName失敗！\n"); return 1; }
    return 0;
    //成功_0, 失敗_1
}
int getVideoName(char *filename, char *id, char *file_v) {
    char file[LEN_FILE+12], url[LEN_URL], command[LEN_CMD];
    sprintf(file, "\"%s_v.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp --get-filename -f bestvideo -o %s %s", file, url);
    puts("正在取得影片檔名...");
    if(getCommandResult(command, file_v)) { puts("getVideoName失敗！\n"); return 1; }
    if(!strstr(file_v, filename))         { puts("getVideoName失敗！\n"); return 1; }
    if(checkFileExist(file_v))            { puts("getVideoName失敗！\n"); return 1; }
    return 0;
    //成功_0, 失敗_1
}

//ffmpeg相關
void ffmpegAudio(char *filename, char *file_a) {
    char command[LEN_CMD];
    if(flagConvertM4A) sprintf(command, "ffmpeg -i \"%s\" \"%s.m4a\"", file_a, filename);
    else               sprintf(command, "ffmpeg -i \"%s\" \"%s.mp3\"", file_a, filename);
    puts("正在轉檔...");
    runCommand(command);
    if(!flagKeepOriginal) {
        puts("正在清除...\n");
        remove(file_a);
    }
}
void ffmpegVideo(char *filename, char *file_a, char *file_v) {
    char command[LEN_CMD];
    sprintf(command, "ffmpeg -i \"%s\" -i \"%s\" -c:a aac -c:v libx264 \"%s.mov\"", file_a, file_v, filename);
    puts("正在轉檔...");
    runCommand(command);
    if(!flagKeepOriginal) {
        puts("正在清除...\n");
        remove(file_a);
        remove(file_v);
    }
}

//下載縮圖
void getThumbnailFile(char *filename, char *id) {
    char url[LEN_URL];
    sprintf(url, "http://img.youtube.com/vi/%s/maxresdefault.jpg", id);
    puts("正在下載縮圖...");
    downloadFile(filename, url);
}

//各模式函式
void modeU(void) {
    puts("[軟體更新模式]\n");
    updateYTDLP();
    updateFFmpeg();
    puts("更新完成！\n");
}
void modeA(void) {
    char id[LEN_VID], filename[LEN_FILE], file_a[LEN_FILE+5];
    if(getURLFromInput("音訊", id)) return;
    if(flagCustomFileName) getCustomFileName(filename);
    else                   strcpy(filename, id);
    if(getAudioFile(filename, id)) return;
    if(!flagNoConvert) {
        if(getAudioName(filename, id, file_a)) return;
        ffmpegAudio(filename, file_a);
    }
    puts("下載完成！\n");
}
void modeV(void) {
    char id[LEN_VID], filename[LEN_FILE], file_a[LEN_FILE+5], file_v[LEN_FILE+5];
    if(getURLFromInput("影片", id)) return;
    if(flagCustomFileName) getCustomFileName(filename);
    else                   strcpy(filename, id);
    if(getAudioFile(filename, id)) return;
    if(getVideoFile(filename, id)) return;
    if(!flagNoConvert) {
        if(getAudioName(filename, id, file_a)) return;
        if(getVideoName(filename, id, file_v)) return;
        ffmpegVideo(filename, file_a, file_v);
    }
    puts("下載完成！\n");
}
void modeT(void) {
    char id[LEN_VID], filename[LEN_FILE];
    if(getURLFromInput("縮圖", id)) return;
    if(flagCustomFileName) getCustomFileName(filename);
    else                   strcpy(filename, id);
    getThumbnailFile(filename, id);
    puts("下載完成！\n");
}
void modeFA(void) {
    int success = 0, fail = 0, status = 0;
    char id[LEN_VID], filename[LEN_FILE], file_a[LEN_FILE+5];
    if(setSourceFile("音訊")) { fclose(URL); fclose(NAME); return; }
    while((status = getDataFromFile(success+fail+1, id, filename))) {
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(status < 0)                 { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        if(getAudioFile(filename, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        if(!flagNoConvert) {
            if(getAudioName(filename, id, file_a)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            ffmpegAudio(filename, file_a);
        }
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("音訊下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL); fclose(NAME);
}
void modeFV(void) {
    int success = 0, fail = 0, status = 0;
    char id[LEN_VID], filename[LEN_FILE], file_a[LEN_FILE+5], file_v[LEN_FILE+5];
    if(setSourceFile("影片")) { fclose(URL); fclose(NAME); return; }
    while((status = getDataFromFile(success+fail+1, id, filename))) {
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(status < 0)                 { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        if(getAudioFile(filename, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        if(getVideoFile(filename, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        if(!flagNoConvert) {
            if(getAudioName(filename, id, file_a)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            if(getVideoName(filename, id, file_v)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            ffmpegVideo(filename, file_a, file_v);
        }
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("影片下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL); fclose(NAME);
}
void modeFT(void) {
    int success = 0, fail = 0, status = 0;
    char id[LEN_VID], filename[LEN_FILE];
    if(setSourceFile("縮圖")) { fclose(URL); fclose(NAME); return; }
    while((status = getDataFromFile(success+fail+1, id, filename))) {
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(status < 0) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        getThumbnailFile(filename, id);
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("縮圖下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL); fclose(NAME);
}

//主函式
int main(int argc, char *argv[]) {
    if(parseArguments(argc, argv)) return 0;
    switch (mode) {
        case 1: modeU();  break;
        case 2: modeA();  break;
        case 3: modeV();  break;
        case 4: modeT();  break;
        case 5: modeFA(); break;
        case 6: modeFV(); break;
        case 7: modeFT(); break;
        default:          break;
    }
    return 0;
}
