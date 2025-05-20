#ifndef BORROW_H
#define BORROW_H

#include "book.h"
#include "student.h"

// 借阅管理函数
int borrowBook(const char* studentId, const char* bookId);
int returnBook(const char* studentId, const char* bookId);
int payCompensation(const char* studentId, const char* bookId);
void displayBorrowedBooks(const char* studentId);
void displayOverdueBooks();
void getCurrentDate(char* dateStr);

#endif // BORROW_H 