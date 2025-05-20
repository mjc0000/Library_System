#ifndef STUDENT_H
#define STUDENT_H

#include "book.h"

#define MAX_STUDENTS 500
#define MAX_STUDENT_NAME_LEN 30
#define MAX_PHONE_LEN 15
#define MAX_BORROWED_BOOKS 5
#define MAX_ID_LEN 20

typedef struct {
    char id[MAX_ID_LEN];                // 学号
    char name[MAX_STUDENT_NAME_LEN];    // 姓名
    char phone[MAX_PHONE_LEN];          // 电话
    char borrowedBooks[MAX_BORROWED_BOOKS][MAX_ID_LEN]; // 借书索引号
    char borrowDates[MAX_BORROWED_BOOKS][11];  // 借书日期 (YYYY-MM-DD)
    int borrowCount;                    // 已借书数量
} Student;

// 学生管理函数
void initStudentSystem();
int addStudent();
int removeStudent();
void displayAllStudents();
Student* findStudentById(const char* id);
void saveStudents();
void loadStudents();

extern Student students[MAX_STUDENTS];
extern int studentCount;

#endif // STUDENT_H 