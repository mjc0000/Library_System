#include <stdio.h>
#include <string.h>
#include "student.h"
#include "fileio.h"

Student students[MAX_STUDENTS];
int studentCount = 0;

void initStudentSystem() {
    studentCount = 0;
    loadStudents();
}

int addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("学生库已满，无法添加更多学生！\n");
        return 0;
    }
    
    Student newStudent;
    
    printf("请输入学生信息：\n");
    printf("学号: ");
    scanf("%s", newStudent.id);
    
    // 检查学号是否已存在
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, newStudent.id) == 0) {
            printf("该学号已存在！\n");
            return 0;
        }
    }
    
    printf("姓名: ");
    scanf("%s", newStudent.name);
    
    printf("电话: ");
    scanf("%s", newStudent.phone);
    
    newStudent.borrowCount = 0;
    
    students[studentCount++] = newStudent;
    printf("学生添加成功！\n");
    
    saveStudents();
    return 1;
}

int removeStudent() {
    char id[MAX_ID_LEN];
    
    printf("请输入要删除的学生学号: ");
    scanf("%s", id);
    
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, id) == 0) {
            if (students[i].borrowCount > 0) {
                printf("该学生有%d本未归还的图书，无法删除！\n", students[i].borrowCount);
                return 0;
            }
            
            // 删除学生（将最后一个学生移到当前位置）
            if (i < studentCount - 1) {
                students[i] = students[studentCount - 1];
            }
            studentCount--;
            
            printf("学生删除成功！\n");
            saveStudents();
            return 1;
        }
    }
    
    printf("未找到该学号的学生！\n");
    return 0;
}

void displayAllStudents() {
    if (studentCount == 0) {
        printf("学生库为空！\n");
        return;
    }
    
    printf("\n%-10s %-20s %-15s %-10s\n", 
           "学号", "姓名", "电话", "已借书数");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        printf("%-10s %-20s %-15s %-10d\n", 
               students[i].id, students[i].name, students[i].phone, students[i].borrowCount);
    }
    printf("\n");
}

Student* findStudentById(const char* id) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, id) == 0) {
            return &students[i];
        }
    }
    return NULL;
}

void saveStudents() {
    if (saveDataToFile(students, sizeof(Student), studentCount, STUDENTS_FILE)) {
        printf("学生数据保存成功！\n");
    } else {
        printf("学生数据保存失败！\n");
    }
}

void loadStudents() {
    studentCount = loadDataFromFile(students, sizeof(Student), MAX_STUDENTS, STUDENTS_FILE);
    if (studentCount > 0) {
        printf("已加载%d名学生数据。\n", studentCount);
    } else {
        printf("没有找到学生数据或数据为空。\n");
        studentCount = 0;
    }
} 