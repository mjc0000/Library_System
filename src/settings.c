#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include "settings.h"

#pragma comment(lib, "winmm.lib")

static bool bgmPlaying = false;
static char currentBGM[MAX_PATH] = "";
static const char* BGM_FOLDER = "bgm";

void initSettingsSystem(void) {
    // 确保BGM文件夹存在
    CreateDirectory(BGM_FOLDER, NULL);
}

void listBGMFiles(void) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    
    printf("\n可用的BGM文件：\n");
    if ((dir = opendir(BGM_FOLDER)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".mp3") || strstr(ent->d_name, ".wav")) {
                printf("%d. %s\n", ++count, ent->d_name);
            }
        }
        closedir(dir);
        
        if (count == 0) {
            printf("没有找到音频文件。请将MP3或WAV文件放入bgm文件夹。\n");
        }
    } else {
        printf("无法打开BGM文件夹。\n");
    }
}

bool playBGM(const char* filename) {
    char fullPath[MAX_PATH];
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", BGM_FOLDER, filename);
    
    // 停止当前播放的BGM
    if (bgmPlaying) {
        stopBGM();
    }
    
    // 使用Windows多媒体API播放音频
    char mciCommand[MAX_PATH + 100];
    snprintf(mciCommand, sizeof(mciCommand), "open \"%s\" type mpegvideo alias bgm", fullPath);
    
    if (mciSendString(mciCommand, NULL, 0, NULL) != 0) {
        printf("无法打开音频文件。\n");
        return false;
    }
    
    if (mciSendString("play bgm repeat", NULL, 0, NULL) != 0) {
        mciSendString("close bgm", NULL, 0, NULL);
        printf("无法播放音频文件。\n");
        return false;
    }
    
    strcpy(currentBGM, filename);
    bgmPlaying = true;
    return true;
}

void stopBGM(void) {
    if (bgmPlaying) {
        mciSendString("stop bgm", NULL, 0, NULL);
        mciSendString("close bgm", NULL, 0, NULL);
        currentBGM[0] = '\0';
        bgmPlaying = false;
    }
}

bool isBGMPlaying(void) {
    return bgmPlaying;
}

void settingsManagement(void) {
    int choice;
    char filename[MAX_PATH];
    
    while (1) {
        printf("\n********** 系统设置 **********\n");
        printf("*  1. BGM设置               *\n");
        printf("*  0. 返回主菜单            *\n");
        printf("****************************\n");
        
        if (bgmPlaying) {
            printf("\n当前正在播放: %s\n", currentBGM);
        }
        
        printf("\n请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                printf("\n********** BGM设置 **********\n");
                printf("1. 播放BGM\n");
                printf("2. 停止播放\n");
                printf("0. 返回\n");
                
                int bgmChoice;
                printf("\n请选择操作: ");
                scanf("%d", &bgmChoice);
                
                switch (bgmChoice) {
                    case 1:
                        listBGMFiles();
                        printf("\n请输入要播放的音频文件名（包含扩展名）: ");
                        scanf("%s", filename);
                        if (playBGM(filename)) {
                            printf("开始播放: %s\n", filename);
                        }
                        break;
                    case 2:
                        stopBGM();
                        printf("已停止播放BGM。\n");
                        break;
                    case 0:
                        break;
                    default:
                        printf("无效的选择！\n");
                }
                break;
            }
            case 0:
                return;
            default:
                printf("无效的选择！\n");
        }
    }
} 