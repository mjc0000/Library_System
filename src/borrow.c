#include <stdio.h>
#include <string.h>
#include <time.h>
#include "borrow.h"
#include "book.h"
#include "student.h"

void getCurrentDate(char* dateStr) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(dateStr, "%04d-%02d-%02d", 
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}

int borrowBook(const char* studentId, const char* bookId) {
    Student* student = findStudentById(studentId);
    if (student == NULL) {
        printf("未找到该学号的学生！\n");
        return 0;
    }
    
    if (student->borrowCount >= MAX_BORROWED_BOOKS) {
        printf("该学生已借阅%d本书，达到最大借阅数量！\n", MAX_BORROWED_BOOKS);
        return 0;
    }
    
    Book* book = findBookById(bookId);
    if (book == NULL) {
        printf("未找到该索引号的图书！\n");
        return 0;
    }
    
    if (book->borrowed >= book->total) {
        printf("该图书已全部借出，无法借阅！\n");
        return 0;
    }
    
    // 检查学生是否已借阅该书
    for (int i = 0; i < student->borrowCount; i++) {
        if (strcmp(student->borrowedBooks[i], bookId) == 0) {
            printf("该学生已借阅此书！\n");
            return 0;
        }
    }
    
    // 借书
    strcpy(student->borrowedBooks[student->borrowCount], bookId);
    getCurrentDate(student->borrowDates[student->borrowCount]);
    student->borrowCount++;
    book->borrowed++;
    
    printf("借书成功！\n");
    saveBooks();
    saveStudents();
    return 1;
}

int returnBook(const char* studentId, const char* bookId) {
    Student* student = findStudentById(studentId);
    if (student == NULL) {
        printf("未找到该学号的学生！\n");
        return 0;
    }
    
    Book* book = findBookById(bookId);
    if (book == NULL) {
        printf("未找到该索引号的图书！\n");
        return 0;
    }
    
    // 查找学生借阅记录
    int found = -1;
    for (int i = 0; i < student->borrowCount; i++) {
        if (strcmp(student->borrowedBooks[i], bookId) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("该学生未借阅此书！\n");
        return 0;
    }
    
    // 还书（将最后一条借阅记录移到当前位置）
    if (found < student->borrowCount - 1) {
        strcpy(student->borrowedBooks[found], student->borrowedBooks[student->borrowCount - 1]);
        strcpy(student->borrowDates[found], student->borrowDates[student->borrowCount - 1]);
    }
    student->borrowCount--;
    book->borrowed--;
    
    printf("还书成功！\n");
    saveBooks();
    saveStudents();
    return 1;
}

int payCompensation(const char* studentId, const char* bookId) {
    Student* student = findStudentById(studentId);
    if (student == NULL) {
        printf("未找到该学号的学生！\n");
        return 0;
    }
    
    Book* book = findBookById(bookId);
    if (book == NULL) {
        printf("未找到该索引号的图书！\n");
        return 0;
    }
    
    // 查找学生借阅记录
    int found = -1;
    for (int i = 0; i < student->borrowCount; i++) {
        if (strcmp(student->borrowedBooks[i], bookId) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("该学生未借阅此书，无法进行赔偿处理！\n");
        return 0;
    }
    
    float compensation = book->price * 1.5; // 赔偿金额为书价的1.5倍
    
    printf("图书《%s》的赔偿金额为：%.2f元\n", book->name, compensation);
    printf("是否确认赔偿？(1-确认/0-取消): ");
    int confirm;
    scanf("%d", &confirm);
    
    if (confirm) {
        // 处理赔偿（将最后一条借阅记录移到当前位置）
        if (found < student->borrowCount - 1) {
            strcpy(student->borrowedBooks[found], student->borrowedBooks[student->borrowCount - 1]);
            strcpy(student->borrowDates[found], student->borrowDates[student->borrowCount - 1]);
        }
        student->borrowCount--;
        
        // 图书总数减1
        book->total--;
        book->borrowed--;
        
        printf("赔偿处理成功！\n");
        saveBooks();
        saveStudents();
        return 1;
    } else {
        printf("已取消赔偿处理。\n");
        return 0;
    }
}

void displayBorrowedBooks(const char* studentId) {
    Student* student = findStudentById(studentId);
    if (student == NULL) {
        printf("未找到该学号的学生！\n");
        return;
    }
    
    if (student->borrowCount == 0) {
        printf("该学生未借阅任何图书！\n");
        return;
    }
    
    printf("\n学号: %s, 姓名: %s 的借阅记录：\n", student->id, student->name);
    printf("%-10s %-30s %-20s %-10s %-15s\n", 
           "索引号", "书名", "作者", "借阅日期", "状态");
    printf("-------------------------------------------------------------------------\n");
    
    char currentDate[11];
    getCurrentDate(currentDate);
    
    for (int i = 0; i < student->borrowCount; i++) {
        Book* book = findBookById(student->borrowedBooks[i]);
        if (book != NULL) {
            // 简单判断是否超期（假设借期为30天）
            char status[20] = "正常";
            
            // 这里可以添加日期比较逻辑来判断是否超期
            // 为简化，这里省略了日期比较的具体实现
            
            printf("%-10s %-30s %-20s %-10s %-15s\n", 
                   book->id, book->name, book->author, 
                   student->borrowDates[i], status);
        }
    }
    printf("\n");
}

void displayOverdueBooks() {
    int hasOverdue = 0;
    char currentDate[11];
    getCurrentDate(currentDate);
    
    printf("\n超期图书列表：\n");
    printf("%-10s %-10s %-20s %-30s %-15s\n", 
           "学号", "姓名", "索引号", "书名", "借阅日期");
    printf("-------------------------------------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        for (int j = 0; j < students[i].borrowCount; j++) {
            // 这里应该有日期比较逻辑来判断是否超期
            // 为简化，假设所有借阅超过30天的都是超期
            // 实际实现中需要计算日期差值
            
            // 简化处理，这里假设所有书都显示
            Book* book = findBookById(students[i].borrowedBooks[j]);
            if (book != NULL) {
                printf("%-10s %-10s %-20s %-30s %-15s\n", 
                       students[i].id, students[i].name, 
                       book->id, book->name, 
                       students[i].borrowDates[j]);
                hasOverdue = 1;
            }
        }
    }
    
    if (!hasOverdue) {
        printf("没有超期图书！\n");
    }
    printf("\n");
} 