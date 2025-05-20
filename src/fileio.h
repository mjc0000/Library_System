#ifndef FILEIO_H
#define FILEIO_H

#include "book.h"
#include "student.h"

// 文件路径
#define BOOKS_FILE "books.dat"
#define STUDENTS_FILE "students.dat"

// 文件操作函数
int saveDataToFile(const void* data, size_t size, size_t count, const char* filename);
int loadDataFromFile(void* data, size_t size, size_t count, const char* filename);

#endif // FILEIO_H 