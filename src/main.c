#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "student.h"
#include "borrow.h"
#include "auth.h"
#include "settings.h"
#include <locale.h>

void displayMainMenu(UserRole role);
void bookManagement();
void studentManagement();
void borrowManagement();
void queryManagement();
void userManagement();
void displayLoginMenu();

// 全局变量保存当前登录用户信息
static char currentUser[MAX_USERNAME_LEN];
static UserRole currentRole;

int main() {
    setlocale(LC_ALL, "");
    // 初始化系统
    initAuthSystem();
    initBookSystem();
    initStudentSystem();
    initSettingsSystem();
    
    bool running = true;
    while (running) {
        // 登录循环
        while (1) {
            displayLoginMenu();
            char username[MAX_USERNAME_LEN];
            char password[MAX_PASSWORD_LEN];
            
            printf("请输入用户名: ");
            scanf("%s", username);
            printf("请输入密码: ");
            scanf("%s", password);
            
            if (login(username, password, &currentRole)) {
                strcpy(currentUser, username);
                printf("登录成功！\n");
                break;
            } else {
                printf("用户名或密码错误，请重试！\n");
            }
        }
        
        // 主菜单循环
        while (1) {
            displayMainMenu(currentRole);
            int choice;
            printf("请选择操作: ");
            scanf("%d", &choice);
            
            if (choice == 6) {  // 切换用户
                printf("正在切换用户...\n");
                break;  // 跳出主菜单循环，返回登录界面
            }
            
            if (choice == 0) {  // 退出系统
                running = false;
                break;
            }
            
            switch (choice) {
                case 1:
                    if (currentRole == ROLE_ADMIN) {
                        bookManagement();
                    } else {
                        printf("权限不足，只有管理员可以进行图书管理！\n");
                    }
                    break;
                case 2:
                    if (currentRole == ROLE_ADMIN) {
                        studentManagement();
                    } else {
                        printf("权限不足，只有管理员可以进行学生管理！\n");
                    }
                    break;
                case 3:
                    borrowManagement();
                    break;
                case 4:
                    queryManagement();
                    break;
                case 5:
                    if (currentRole == ROLE_ADMIN) {
                        userManagement();
                    } else {
                        // 读者只能修改自己的密码
                        char oldPassword[MAX_PASSWORD_LEN];
                        char newPassword[MAX_PASSWORD_LEN];
                        printf("请输入旧密码: ");
                        scanf("%s", oldPassword);
                        printf("请输入新密码: ");
                        scanf("%s", newPassword);
                        
                        if (changePassword(currentUser, oldPassword, newPassword)) {
                            printf("密码修改成功！\n");
                        } else {
                            printf("密码修改失败，请确认旧密码是否正确！\n");
                        }
                    }
                    break;
                case 7:
                    // 调用设置管理功能的逻辑
                    settingsManagement();
                    break;
                default:
                    printf("无效的选择，请重新输入！\n");
            }
        }
    }
    
    // 退出前停止BGM
    stopBGM();
    printf("感谢使用图书馆借阅系统，再见！\n");
    return 0;
}

void displayLoginMenu() {
    printf("\n********** 图书馆借阅系统 **********\n");
    printf("*          请登录后使用           *\n");
    printf("************************************\n");
}

void displayMainMenu(UserRole role) {
    printf("\n********** 图书馆借阅系统 **********\n");
    if (role == ROLE_ADMIN) {
        printf("*  1. 图书管理                     *\n");
        printf("*  2. 学生管理                     *\n");
        printf("*  3. 借阅管理                     *\n");
        printf("*  4. 查询统计                     *\n");
        printf("*  5. 用户管理                     *\n");
    } else {
        printf("*  3. 借阅管理                     *\n");
        printf("*  4. 查询统计                     *\n");
        printf("*  5. 修改密码                     *\n");
    }
    printf("*  6. 切换用户                     *\n");
    printf("*  7. 系统设置                     *\n");
    printf("*  0. 退出系统                     *\n");
    printf("************************************\n");
}

void bookManagement() {
    int choice;
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    Book* book;
    
    while (1) {
        printf("\n********** 图书管理 **********\n");
        printf("*  1. 添加图书               *\n");
        printf("*  2. 删除图书               *\n");
        printf("*  3. 显示所有图书           *\n");
        printf("*  4. 查找图书(按索引号)     *\n");
        printf("*  5. 查找图书(按书名)       *\n");
        printf("*  0. 返回主菜单             *\n");
        printf("******************************\n");
        
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                removeBook();
                break;
            case 3:
                displayAllBooks();
                break;
            case 4:
                printf("请输入图书索引号: ");
                scanf("%s", id);
                book = findBookById(id);
                if (book != NULL) {
                    printf("\n找到图书：\n");
                    printf("索引号: %s\n", book->id);
                    printf("书名: %s\n", book->name);
                    printf("作者: %s\n", book->author);
                    printf("出版社: %s\n", book->publisher);
                    printf("单价: %.2f\n", book->price);
                    printf("库存: %d\n", book->total);
                    printf("借出: %d\n", book->borrowed);
                } else {
                    printf("未找到该索引号的图书！\n");
                }
                break;
            case 5:
                printf("请输入图书名称: ");
                scanf("%s", name);
                book = findBookByName(name);
                if (book != NULL) {
                    printf("\n找到图书：\n");
                    printf("索引号: %s\n", book->id);
                    printf("书名: %s\n", book->name);
                    printf("作者: %s\n", book->author);
                    printf("出版社: %s\n", book->publisher);
                    printf("单价: %.2f\n", book->price);
                    printf("库存: %d\n", book->total);
                    printf("借出: %d\n", book->borrowed);
                } else {
                    printf("未找到该名称的图书！\n");
                }
                break;
            case 0:
                return;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}

void studentManagement() {
    int choice;
    char id[MAX_ID_LEN];
    Student* student;
    
    while (1) {
        printf("\n********** 学生管理 **********\n");
        printf("*  1. 添加学生               *\n");
        printf("*  2. 删除学生               *\n");
        printf("*  3. 显示所有学生           *\n");
        printf("*  4. 查找学生               *\n");
        printf("*  0. 返回主菜单             *\n");
        printf("******************************\n");
        
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                removeStudent();
                break;
            case 3:
                displayAllStudents();
                break;
            case 4:
                printf("请输入学生学号: ");
                scanf("%s", id);
                student = findStudentById(id);
                if (student != NULL) {
                    printf("\n找到学生：\n");
                    printf("学号: %s\n", student->id);
                    printf("姓名: %s\n", student->name);
                    printf("电话: %s\n", student->phone);
                    printf("已借书数: %d\n", student->borrowCount);
                    
                    if (student->borrowCount > 0) {
                        printf("\n借阅的图书：\n");
                        for (int i = 0; i < student->borrowCount; i++) {
                            Book* book = findBookById(student->borrowedBooks[i]);
                            if (book != NULL) {
                                printf("%d. %s (%s), 借阅日期: %s\n", 
                                       i+1, book->name, book->id, student->borrowDates[i]);
                            }
                        }
                    }
                } else {
                    printf("未找到该学号的学生！\n");
                }
                break;
            case 0:
                return;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}

void borrowManagement() {
    int choice;
    char studentId[MAX_ID_LEN];
    char bookId[MAX_ID_LEN];
    
    while (1) {
        printf("\n********** 借阅管理 **********\n");
        printf("*  1. 借书                   *\n");
        printf("*  2. 还书                   *\n");
        printf("*  3. 遗失赔偿               *\n");
        printf("*  4. 查看学生借阅记录       *\n");
        printf("*  5. 查看超期图书           *\n");
        printf("*  0. 返回主菜单             *\n");
        printf("******************************\n");
        
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("请输入学生学号: ");
                scanf("%s", studentId);
                printf("请输入图书索引号: ");
                scanf("%s", bookId);
                borrowBook(studentId, bookId);
                break;
            case 2:
                printf("请输入学生学号: ");
                scanf("%s", studentId);
                printf("请输入图书索引号: ");
                scanf("%s", bookId);
                returnBook(studentId, bookId);
                break;
            case 3:
                printf("请输入学生学号: ");
                scanf("%s", studentId);
                printf("请输入图书索引号: ");
                scanf("%s", bookId);
                payCompensation(studentId, bookId);
                break;
            case 4:
                printf("请输入学生学号: ");
                scanf("%s", studentId);
                displayBorrowedBooks(studentId);
                break;
            case 5:
                displayOverdueBooks();
                break;
            case 0:
                return;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}

void queryManagement() {
    int choice;
    
    while (1) {
        printf("\n********** 查询统计 **********\n");
        printf("*  1. 查询图书借阅情况       *\n");
        printf("*  2. 查询学生借阅情况       *\n");
        printf("*  3. 统计图书库存情况       *\n");
        printf("*  0. 返回主菜单             *\n");
        printf("******************************\n");
        
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                displayAllBooks();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                printf("\n图书库存统计：\n");
                printf("总图书种类: %d\n", bookCount);
                
                int totalBooks = 0;
                int borrowedBooks = 0;
                for (int i = 0; i < bookCount; i++) {
                    totalBooks += books[i].total;
                    borrowedBooks += books[i].borrowed;
                }
                
                printf("总图书数量: %d\n", totalBooks);
                printf("已借出数量: %d\n", borrowedBooks);
                printf("可借阅数量: %d\n", totalBooks - borrowedBooks);
                break;
            case 0:
                return;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}

void userManagement() {
    if (currentRole != ROLE_ADMIN) {
        printf("权限不足！\n");
        return;
    }
    
    int choice;
    while (1) {
        printf("\n********** 用户管理 **********\n");
        printf("*  1. 添加用户               *\n");
        printf("*  2. 修改密码               *\n");
        printf("*  0. 返回主菜单             *\n");
        printf("****************************\n");
        
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char username[MAX_USERNAME_LEN];
                char password[MAX_PASSWORD_LEN];
                int roleChoice;
                
                printf("请输入用户名: ");
                scanf("%s", username);
                printf("请输入密码: ");
                scanf("%s", password);
                printf("请选择用户角色 (0:管理员, 1:读者): ");
                scanf("%d", &roleChoice);
                
                if (addUser(username, password, roleChoice == 0 ? ROLE_ADMIN : ROLE_READER)) {
                    printf("用户添加成功！\n");
                } else {
                    printf("用户添加失败，可能是用户名已存在或达到最大用户数！\n");
                }
                break;
            }
            case 2: {
                char username[MAX_USERNAME_LEN];
                char newPassword[MAX_PASSWORD_LEN];
                
                printf("请输入要修改密码的用户名: ");
                scanf("%s", username);
                printf("请输入新密码: ");
                scanf("%s", newPassword);
                
                if (changePassword(username, "", newPassword)) {
                    printf("密码修改成功！\n");
                } else {
                    printf("密码修改失败，用户可能不存在！\n");
                }
                break;
            }
            case 0:
                return;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
} 