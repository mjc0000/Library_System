#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

// 初始化设置系统
void initSettingsSystem(void);

// 显示设置菜单
void settingsManagement(void);

// 播放BGM
bool playBGM(const char* filename);

// 停止BGM
void stopBGM(void);

// 检查BGM是否正在播放
bool isBGMPlaying(void);

#endif // SETTINGS_H 