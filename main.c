#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define URL_LEN 100
#define ID_LEN 20
#define FILE_LEN 200
#define CMD_LEN 500

//全域變數
char mode = 0;
char keep_original = 0;
char no_convert = 0;
char custom_filename = 0;
char convert_m4a = 0;
char print_command = 0;

//輸出提示資訊
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
    puts("[-c]   自訂檔案名稱\t\t(在 a, v, t 模式可用)");
    puts("[-m4a] 轉檔為 m4a 格式\t\t(在 a, fa 模式可用)");
    puts("[-p]   輸出執行的指令\t\t(在所有模式可用)");
    puts("");
}

//處理命令列參數
int checkArguments(char arg) {
    if(arg > 5) return 0;
    else if(arg == 1 && (mode == 2 || mode == 3 || mode == 5 || mode == 6)) return 1;
    else if(arg == 2 && (mode == 2 || mode == 3 || mode == 5 || mode == 6)) return 1;
    else if(arg == 3 && (mode == 2 || mode == 3 || mode == 4)) return 1;
    else if(arg == 4 && (mode == 2 || mode == 5 )) return 1;
    else if(arg == 5) return 1;
    else return 0;
}
int parseArguments(int argc, char *argv[]) {
    if(argc == 1) {
        printf("歡迎使用YouTube下載輔助工具！\n");
        printf("作者：czs940902\n版本：v2.1 (20250202.1)\n\n");
        printAvailableMode();
        printAvailableArguments();
        printf("範例指令：\"dl a\", \"dl v -k\"\n\n");
        return 0;
    }
    else if(strcmp(argv[1], "u") == 0) mode = 1;
    else if(strcmp(argv[1], "a") == 0) mode = 2;
    else if(strcmp(argv[1], "v") == 0) mode = 3;
    else if(strcmp(argv[1], "t") == 0) mode = 4;
    else if(strcmp(argv[1], "fa") == 0) mode = 5;
    else if(strcmp(argv[1], "fv") == 0) mode = 6;
    else if(strcmp(argv[1], "ft") == 0) mode = 7;
    else { printf("模式錯誤！"); printAvailableMode(); return 0; }
    if(argc > 2) {
        for(int i=2;i<argc;i++) {
            if(strcmp(argv[i], "-k") == 0 && checkArguments(1)) { keep_original = 1; }
            else if(strcmp(argv[i], "-n") == 0 && checkArguments(2)) { no_convert = 1; keep_original = 1; }
            else if(strcmp(argv[i], "-c") == 0 && checkArguments(3)) { custom_filename = 1; }
            else if(strcmp(argv[i], "-m4a") == 0 && checkArguments(4)) { convert_m4a = 1; }
            else if(strcmp(argv[i], "-p") == 0 && checkArguments(5)) { print_command = 1; }
            else { printf("無效的參數：%s\n\n", argv[i]); printAvailableArguments(); return 0; }
        }
        printf("使用參數："); for(int i=2;i<argc;i++) printf("%s ", argv[i]); puts(""); puts("");
    }
    return 1;
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
    } else { printf("連結錯誤！\n\n"); return 1; }
    return 0;
}
int getURLFromInput(char *mode, char *id) {
    char buffer[URL_LEN];
    printf("[%s下載模式]\n請輸入YouTube連結：", mode);
    fgets(buffer, URL_LEN, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(getVideoIDFromURL(buffer, id)) return 1;
    printf("將下載YouTube%s：https://youtu.be/%s\n\n", mode, id);
    return 0;
}

//自訂檔名輸入
void getCustomFileName(char *filename) {
    printf("請輸入自訂檔案名稱：");
    fgets(filename, FILE_LEN, stdin);
    filename[strcspn(filename, "\n")] = 0;
    printf("將使用自訂檔案名稱：%s\n\n", filename);
}

//yt-dlp相關
void getAudioFile(char *filename, char *id) {
    char file[FILE_LEN+12], url[URL_LEN], command[CMD_LEN];
    sprintf(file, "\"%s_a.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp -f bestaudio -o %s %s", file, url);
    printf("正在下載音訊部分...\n");
    if(print_command) printf("[指令] %s\n", command);
    system(command); puts("");
}
void getVideoFile(char *filename, char *id) {
    char file[FILE_LEN+12], url[URL_LEN], command[CMD_LEN];
    sprintf(file, "\"%s_v.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp -f bestvideo -o %s %s", file, url);
    printf("正在下載影片部分...\n");
    if(print_command) printf("[指令] %s\n", command);
    system(command); puts("");
}
int getAudioName(char *filename, char *id, char *file_a) {
    char file[FILE_LEN+12], url[URL_LEN], command[CMD_LEN];
    sprintf(file, "\"%s_a.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp --get-filename -f bestaudio -o %s %s", file, url);
    printf("正在取得音訊檔名...\n");
    if(print_command) printf("[指令] %s\n", command);
    FILE *fp = popen(command, "r"); puts("");
    if(!fp) { printf("getAudioName失敗！\n\n"); return 1; }
    if(fgets(file_a, FILE_LEN+5, fp)) file_a[strcspn(file_a, "\n")] = 0;
    if(!strstr(file_a, filename)) { printf("下載失敗！\n\n"); return 1; }
    if(access(file_a, F_OK)) { printf("下載失敗！\n\n"); return 1; }
    pclose(fp); return 0;
}
int getVideoName(char *filename, char *id, char *file_v) {
    char file[FILE_LEN+12], url[URL_LEN], command[CMD_LEN];
    sprintf(file, "\"%s_v.%%(ext)s\"", filename);
    sprintf(url, "\"https://youtu.be/%s\"", id);
    sprintf(command, "yt-dlp --get-filename -f bestvideo -o %s %s", file, url);
    printf("正在取得影片檔名...\n");
    if(print_command) printf("[指令] %s\n", command);
    FILE *fp = popen(command, "r"); puts("");
    if(!fp) { printf("getVideoName失敗！\n\n"); return 1; }
    if(fgets(file_v, FILE_LEN+5, fp)) file_v[strcspn(file_v, "\n")] = 0;
    if(!strstr(file_v, filename)) { printf("下載失敗！\n\n"); return 1; }
    if(access(file_v, F_OK)) { printf("下載失敗！\n\n"); return 1; }
    pclose(fp); return 0;
}

//ffmpeg相關
void ffmpegAudio(char *filename, char *file_a) {
    char command[CMD_LEN];
    if(convert_m4a) sprintf(command, "ffmpeg -i \"%s\" \"%s.m4a\"", file_a, filename);
    else            sprintf(command, "ffmpeg -i \"%s\" \"%s.mp3\"", file_a, filename);
    printf("正在轉檔...\n");
    if(print_command) printf("[指令] %s\n", command);
    system(command); puts("");
    if(!keep_original) {
        sprintf(command, "rm \"%s\"", file_a);
        printf("正在清除...\n");
        if(print_command) printf("[指令] %s\n", command);
        system(command); puts("");
    }
}
void ffmpegVideo(char *filename, char *file_a, char *file_v) {
    char command[CMD_LEN];
    sprintf(command, "ffmpeg -i \"%s\" -i \"%s\" -c:a aac -c:v libx264 \"%s.mov\"", file_a, file_v, filename);
    printf("正在轉檔...\n");
    if(print_command) printf("[指令] %s\n", command);
    system(command); puts("");
    if(!keep_original) {
        sprintf(command, "rm \"%s\" \"%s\"", file_a, file_v);
        printf("正在清除...\n");
        if(print_command) printf("[指令] %s\n", command);
        system(command); puts("");
    }
}

//下載縮圖
void getThumbnailFile(char *filename, char *id) {
    char url[URL_LEN], command[CMD_LEN];
    sprintf(url, "http://img.youtube.com/vi/%s/maxresdefault.jpg", id);
    sprintf(command, "curl -s \"%s\" -o \"%s.jpg\"", url, filename);
    printf("正在下載縮圖...\n");
    if(print_command) printf("[指令] %s\n", command);
    system(command); puts("");
}

//各模式函式
void modeU(void) {
    printf("[軟體更新模式]\n");
    printf("\n正在更新yt-dlp...\n");
    if(print_command) printf("[指令] brew upgrade yt-dlp\n");
    system("brew upgrade yt-dlp");
    printf("\n正在更新ffmpeg...\n");
    if(print_command) printf("[指令] brew upgrade ffmpeg\n");
    system("brew upgrade ffmpeg");
    printf("\n更新完成！\n\n");
}
void modeA(void) {
    char id[ID_LEN], filename[FILE_LEN], file_a[FILE_LEN+5];
    if(getURLFromInput("音訊", id)) return;
    if(custom_filename) getCustomFileName(filename);
    else                strcpy(filename, id);
    getAudioFile(filename, id);
    if(!no_convert) {
        if(getAudioName(filename, id, file_a)) return;
        ffmpegAudio(filename, file_a);
    }
    printf("下載完成！\n\n");
}
void modeV(void) {
    char id[ID_LEN], filename[FILE_LEN], file_a[FILE_LEN+5], file_v[FILE_LEN+5];
    if(getURLFromInput("影片", id)) return;
    if(custom_filename) getCustomFileName(filename);
    else                strcpy(filename, id);
    getAudioFile(filename, id);
    getVideoFile(filename, id);
    if(!no_convert) {
        if(getAudioName(filename, id, file_a)) return;
        if(getVideoName(filename, id, file_v)) return;
        ffmpegVideo(filename, file_a, file_v);
    }
    printf("下載完成！\n\n");
}
void modeT(void) {
    char id[ID_LEN], filename[FILE_LEN];
    if(getURLFromInput("縮圖", id)) return;
    if(custom_filename) getCustomFileName(filename);
    else                strcpy(filename, id);
    getThumbnailFile(filename, id);
    printf("下載完成！\n\n");
}
void modeFA(void) {
    int success = 0, fail = 0;
    char buffer[URL_LEN], id[ID_LEN], filename[FILE_LEN], file_a[FILE_LEN+5];
    printf("[讀檔下載模式(音訊)]\n請輸入檔案名稱：");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    FILE *URL = fopen(buffer, "r");
    if(!URL) { printf("檔案錯誤！\n\n"); return; }
    printf("將從此檔案讀取：%s\n\n", buffer);
    while(fgets(buffer, URL_LEN, URL)) {
        buffer[strcspn(buffer, "\n")] = 0;
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(getVideoIDFromURL(buffer, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        sprintf(filename, "%d_%s", success+fail+1, id);
        getAudioFile(filename, id);
        if(!no_convert) {
            if(getAudioName(filename, id, file_a)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            ffmpegAudio(filename, file_a);
        }
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("音訊下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL);
}
void modeFV(void) {
    int success = 0, fail = 0;
    char buffer[URL_LEN], id[ID_LEN], filename[FILE_LEN], file_a[FILE_LEN+5], file_v[FILE_LEN+5];
    printf("[讀檔下載模式(影片)]\n請輸入檔案名稱：");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    FILE *URL = fopen(buffer, "r");
    if(!URL) { printf("檔案錯誤！\n\n"); return; }
    printf("將從此檔案讀取：%s\n\n", buffer);
    while(fgets(buffer, URL_LEN, URL)) {
        buffer[strcspn(buffer, "\n")] = 0;
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(getVideoIDFromURL(buffer, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        sprintf(filename, "%d_%s", success+fail+1, id);
        getAudioFile(filename, id);
        getVideoFile(filename, id);
        if(!no_convert) {
            if(getAudioName(filename, id, file_a)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            if(getVideoName(filename, id, file_v)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
            ffmpegVideo(filename, file_a, file_v);
        }
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("影片下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL);
}
void modeFT(void) {
    int success = 0, fail = 0;
    char buffer[URL_LEN], id[ID_LEN], filename[FILE_LEN];
    printf("[讀檔下載模式(縮圖)]\n請輸入檔案名稱：");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    FILE *URL = fopen(buffer, "r");
    if(!URL) { printf("檔案錯誤！\n\n"); return; }
    printf("將從此檔案讀取：%s\n\n", buffer);
    while(fgets(buffer, URL_LEN, URL)) {
        buffer[strcspn(buffer, "\n")] = 0;
        printf("第 %d 次下載...\n\n", success+fail+1);
        if(getVideoIDFromURL(buffer, id)) { fail++; printf("第 %d 次下載失敗！\n\n", success+fail); continue; }
        sprintf(filename, "%d_%s", success+fail+1, id);
        getThumbnailFile(filename, id);
        success++; printf("第 %d 次下載完成！\n\n", success+fail);
    }
    printf("縮圖下載完成！\n共下載 %d 次，成功 %d 次，失敗 %d 次\n\n", success+fail, success, fail);
    fclose(URL);
}

//主函式
int main(int argc, char *argv[]) {
    if(parseArguments(argc, argv)) {
        switch (mode) {
            case 1: modeU(); break;
            case 2: modeA(); break;
            case 3: modeV(); break;
            case 4: modeT(); break;
            case 5: modeFA(); break;
            case 6: modeFV(); break;
            case 7: modeFT(); break;
            default: break;
        }
    }
    return 0;
}
