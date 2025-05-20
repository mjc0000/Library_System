#ifndef BOOK_H
#define BOOK_H

#define MAX_BOOKS 1000
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MAX_AUTHOR_LEN 30
#define MAX_PUBLISHER_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];        // 书名
    char id[MAX_ID_LEN];            // 索引号
    char author[MAX_AUTHOR_LEN];    // 作者
    char publisher[MAX_PUBLISHER_LEN]; // 出版社
    float price;                    // 单价
    int total;                      // 库存总数
    int borrowed;                   // 借出数量
} Book;

// 图书管理函数
void initBookSystem();
int addBook();
int removeBook();
void displayAllBooks();
Book* findBookById(const char* id);
Book* findBookByName(const char* name);
void saveBooks();
void loadBooks();

extern Book books[MAX_BOOKS];
extern int bookCount;

#endif // BOOK_H 