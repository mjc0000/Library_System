#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_LEN 32
#define MAX_USERS 100

typedef enum {
    ROLE_ADMIN,
    ROLE_READER
} UserRole;

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
} User;

// 初始化认证系统
void initAuthSystem(void);

// 用户登录
bool login(const char* username, const char* password, UserRole* role);

// 修改密码
bool changePassword(const char* username, const char* oldPassword, const char* newPassword);

// 添加新用户（仅管理员可用）
bool addUser(const char* username, const char* password, UserRole role);

// 保存用户数据
void saveUsers(void);

// 加载用户数据
void loadUsers(void);

#endif // AUTH_H 