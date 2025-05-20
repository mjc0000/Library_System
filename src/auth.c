#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auth.h"

static User users[MAX_USERS];
static int userCount = 0;
static const char* USER_FILE = "users.dat";

void initAuthSystem(void) {
    // 加载现有用户
    loadUsers();
    
    // 如果没有用户，创建默认管理员账户
    if (userCount == 0) {
        strcpy(users[0].username, "root");
        strcpy(users[0].password, "123456");
        users[0].role = ROLE_ADMIN;
        userCount = 1;
        saveUsers();
    }
}

bool login(const char* username, const char* password, UserRole* role) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            *role = users[i].role;
            return true;
        }
    }
    return false;
}

bool changePassword(const char* username, const char* oldPassword, const char* newPassword) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, oldPassword) == 0) {
            strcpy(users[i].password, newPassword);
            saveUsers();
            return true;
        }
    }
    return false;
}

bool addUser(const char* username, const char* password, UserRole role) {
    if (userCount >= MAX_USERS) {
        return false;
    }
    
    // 检查用户名是否已存在
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return false;
        }
    }
    
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].role = role;
    userCount++;
    
    saveUsers();
    return true;
}

void saveUsers(void) {
    FILE* file = fopen(USER_FILE, "wb");
    if (file == NULL) {
        printf("Error: Cannot open users file for writing\n");
        return;
    }
    
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fclose(file);
}

void loadUsers(void) {
    FILE* file = fopen(USER_FILE, "rb");
    if (file == NULL) {
        userCount = 0;
        return;
    }
    
    fread(&userCount, sizeof(int), 1, file);
    fread(users, sizeof(User), userCount, file);
    fclose(file);
} 