#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char mode = 0;
int keep_original = 0, no_convert = 0, convert_m4a = 0;
int parseArguments(int argc, char *argv[]) {
    if(argc == 1) {
        puts("歡迎使用YouTube下載工具！");
        puts("作者：czs940902    版本：20250201"); puts("");
        puts("可用模式：[更新] u, [音訊] a, [影片] v, [縮圖] t, [讀檔] f");
        puts("可用參數：[跳過轉檔] -n, [保留原始檔案] -k, [音訊轉檔為m4a] -m4a");
        puts("範例指令：\"dl v -k\", \"dl a -m4a\"");
        return 0;
    }
    else if(strcmp(argv[1], "u") == 0) mode = 'u';
    else if(strcmp(argv[1], "a") == 0) mode = 'a';
    else if(strcmp(argv[1], "v") == 0) mode = 'v';
    else if(strcmp(argv[1], "t") == 0) mode = 't';
    else if(strcmp(argv[1], "f") == 0) mode = 'f';
    else {
        puts("模式錯誤！");
        puts("可用模式：[更新] u, [音訊] a, [影片] v, [縮圖] t, [讀檔] f");
        return 0;
    }
    if(argc > 2) {
        for(int i=2;i<argc;i++) {
            if(strcmp(argv[i], "-k") == 0) { keep_original = 1; }
            else if(strcmp(argv[i], "-n") == 0) { no_convert = 1; keep_original = 1; }
            else if(strcmp(argv[i], "-m4a") == 0) { convert_m4a = 1; }
            else {
                printf("無效的參數：%s\n", argv[i]);
                puts("可用參數：[跳過轉檔] -n, [保留原始檔案] -k, [音訊轉檔為m4a] -m4a");
                return 0;
            }
        }
        printf("使用參數：");
        for(int i=2;i<argc;i++) printf("%s ", argv[i]);
        puts("");
    }
    return 1;
}
int getVideoID(char *buffer, char *id) {
    char *start = NULL;
    if((start = strstr(buffer, "youtu.be/"))) {
        sscanf(start+9, "%11s", id);
    } else if((start = strstr(buffer, "watch?v="))) {
        sscanf(start+8, "%11s", id);
    } else if((start = strstr(buffer, "/live/"))) {
        sscanf(start+6, "%11s", id);
    } else { puts("連結錯誤！"); return 1; }
    return 0;
}
int getURLFromInput(char *mode, char *id) {
    char buffer[100];
    printf("[%s下載模式]\n請輸入YouTube連結：", mode);
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(getVideoID(buffer, id)) return 1;
    printf("將下載YouTube%s：https://youtu.be/%s\n", mode, id);
    return 0;
}
int getAudioFileName(char *id, char *filename) {
    char command[200];
    sprintf(command, "yt-dlp --get-filename -f bestaudio -o \"%s_a.%%(ext)s\" \"https://youtu.be/%s\"", id, id);
    FILE *fp = popen(command, "r");
    if(!fp) { printf("\ngetAudioFileName失敗！\n"); return 1; }
    if(fgets(filename, 20, fp)) filename[strcspn(filename, "\n")] = 0;
    if(!strstr(filename, id)) { printf("\n下載失敗！\n"); return 1; }
    if(access(filename, F_OK)) { printf("\n下載失敗！\n"); return 1; }
    pclose(fp); return 0;
}
int getVideoFileName(char *id, char *filename) {
    char command[200];
    sprintf(command, "yt-dlp --get-filename -f bestvideo -o \"%s_v.%%(ext)s\" \"https://youtu.be/%s\"", id, id);
    FILE *fp = popen(command, "r");
    if(!fp) { printf("\ngetVideoFileName失敗！\n"); return 1; }
    if(fgets(filename, 20, fp)) filename[strcspn(filename, "\n")] = 0;
    if(!strstr(filename, id)) { printf("\n下載失敗！\n"); return 1; }
    if(access(filename, F_OK)) { printf("\n下載失敗！\n"); return 1; }
    pclose(fp); return 0;
}
void downloadAudioFile(char *id) {
    char command[200];
    sprintf(command, "yt-dlp -f bestaudio -o \"%s_a.%%(ext)s\" \"https://youtu.be/%s\"", id, id);
    printf("\n正在下載音訊部分...\n"); system(command);
}
void downloadVideoFile(char *id) {
    char command[200];
    sprintf(command, "yt-dlp -f bestvideo -o \"%s_v.%%(ext)s\" \"https://youtu.be/%s\"", id, id);
    printf("\n正在下載影片部分...\n"); system(command);
}
void downloadThumbnailFile(char *id) {
    char link[100], command[200];
    sprintf(link, "http://img.youtube.com/vi/%s/maxresdefault.jpg", id);
    sprintf(command, "curl -s \"%s\" -o \"%s.jpg\"", link, id);
    printf("\n正在下載縮圖...\n"); system(command);
}
void ffmpegAudio(char *filename, char *id) {
    char command[200];
    if(convert_m4a) {
        sprintf(command, "ffmpeg -i \"%s\" \"%s.m4a\"", filename, id);
        printf("\n正在轉檔...\n"); system(command);
    } else {
        sprintf(command, "ffmpeg -i \"%s\" \"%s.mp3\"", filename, id);
        printf("\n正在轉檔...\n"); system(command);
    }
    if(!keep_original) {
        sprintf(command, "rm \"%s\"", filename);
        printf("\n正在清除...\n"); system(command);
    }
}
void ffmpegVideo(char *file_a, char *file_v, char *id) {
    char command[200];
    sprintf(command, "ffmpeg -i \"%s\" -i \"%s\" -c:a aac -c:v libx264 \"%s.mov\"", file_a, file_v, id);
    printf("\n正在轉檔...\n"); system(command);
    if(!keep_original) {
        sprintf(command, "rm \"%s\" \"%s\"", file_a, file_v);
        printf("\n正在清除...\n"); system(command);
    }
}
void updateSoftware(void) {
    puts("[軟體更新模式]");
    printf("\n正在更新yt-dlp...\n");
    system("brew upgrade yt-dlp");
    printf("\n正在更新ffmpeg...\n");
    system("brew upgrade ffmpeg");
    printf("\n更新完成！\n");
}
void downloadAudio(void) {
    char id[20], filename[20];
    if(getURLFromInput("音訊", id)) return;
    downloadAudioFile(id);
    if(!no_convert) {
        if(getAudioFileName(id, filename)) return;
        ffmpegAudio(filename, id);
    }
    printf("\n下載完成！\n");
}
void downloadVideo(void) {
    char id[20], file_a[20], file_v[20];
    if(getURLFromInput("影片", id)) return;
    downloadAudioFile(id);
    downloadVideoFile(id);
    if(!no_convert) {
        if(getAudioFileName(id, file_a)) return;
        if(getVideoFileName(id, file_v)) return;
        ffmpegVideo(file_a, file_v, id);
    }
    printf("\n下載完成！\n");
}
void downloadThumbnail(void) {
    char id[20];
    if(getURLFromInput("縮圖", id)) return;
    downloadThumbnailFile(id);
    printf("\n下載完成！\n");
}
void downloadFromFile(void) {
    int count = 0;
    char buffer[100], id[20], file_a[50], file_v[50];
    printf("[讀檔下載模式]\n請輸入檔案名稱：");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    FILE *URL = fopen(buffer, "r");
    if(!URL) { puts("檔案錯誤！"); return; }
    fgets(buffer, 100, URL);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strcmp(buffer, "a") == 0) {
        while(fgets(buffer, 100, URL)) {
            buffer[strcspn(buffer, "\n")] = 0;
            printf("\n第 %d 次下載...\n", count+1);
            if(getVideoID(buffer, id)) continue;
            downloadAudioFile(id);
            if(!no_convert) {
                if(getAudioFileName(id, file_a)) continue;
                ffmpegAudio(file_a, id);
            }
            count++;
            printf("\n第 %d 次下載完成！\n", count);
        }
        printf("\n下載完成！共完成 %d 次音訊下載\n", count);
    } else if(strcmp(buffer, "v") == 0) {
        while(fgets(buffer, 100, URL)) {
            buffer[strcspn(buffer, "\n")] = 0;
            printf("\n第 %d 次下載...\n", count+1);
            if(getVideoID(buffer, id)) continue;
            downloadAudioFile(id);
            downloadVideoFile(id);
            if(!no_convert) {
                if(getAudioFileName(id, file_a)) continue;
                if(getVideoFileName(id, file_v)) continue;
                ffmpegVideo(file_a, file_v, id);
            }
            count++;
            printf("\n第 %d 次下載完成！\n", count);
        }
        printf("\n下載完成！共完成 %d 次影片下載\n", count);
    } else if(strcmp(buffer, "t") == 0) {
        while(fgets(buffer, 100, URL)) {
            buffer[strcspn(buffer, "\n")] = 0;
            printf("\n第 %d 次下載...\n", count+1);
            if(getVideoID(buffer, id)) continue;
            downloadThumbnailFile(id);
            count++;
            printf("\n第 %d 次下載完成！\n", count);
        }
        printf("\n下載完成！共完成 %d 次縮圖下載\n", count);
    } else
        printf("模式錯誤！\n可用模式：[音訊] a, [影片] v, [縮圖] t\n");
    fclose(URL);
}
int main(int argc, char *argv[]) {
    if(parseArguments(argc, argv)) {
        switch (mode) {
            case 'u': updateSoftware(); break;
            case 'a': downloadAudio(); break;
            case 'v': downloadVideo(); break;
            case 't': downloadThumbnail(); break;
            case 'f': downloadFromFile(); break;
            default: break;
        }
    }
    return 0;
}
