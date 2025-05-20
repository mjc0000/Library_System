#include <stdio.h>
#include <string.h>
#include "book.h"
#include "fileio.h"

Book books[MAX_BOOKS];
int bookCount = 0;

void initBookSystem() {
    bookCount = 0;
    loadBooks();
}

int addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("图书库已满，无法添加更多图书！\n");
        return 0;
    }
    
    Book newBook;
    
    printf("请输入图书信息：\n");
    printf("索引号: ");
    scanf("%s", newBook.id);
    
    // 检查索引号是否已存在
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].id, newBook.id) == 0) {
            printf("该索引号已存在！\n");
            return 0;
        }
    }
    
    printf("书名: ");
    scanf("%s", newBook.name);
    
    printf("作者: ");
    scanf("%s", newBook.author);
    
    printf("出版社: ");
    scanf("%s", newBook.publisher);
    
    printf("单价: ");
    scanf("%f", &newBook.price);
    
    printf("库存数量: ");
    scanf("%d", &newBook.total);
    
    newBook.borrowed = 0;
    
    books[bookCount++] = newBook;
    printf("图书添加成功！\n");
    
    saveBooks();
    return 1;
}

int removeBook() {
    char id[MAX_ID_LEN];
    
    printf("请输入要删除的图书索引号: ");
    scanf("%s", id);
    
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].id, id) == 0) {
            if (books[i].borrowed > 0) {
                printf("该图书有%d本正在借出，无法删除！\n", books[i].borrowed);
                return 0;
            }
            
            // 删除图书（将最后一本书移到当前位置）
            if (i < bookCount - 1) {
                books[i] = books[bookCount - 1];
            }
            bookCount--;
            
            printf("图书删除成功！\n");
            saveBooks();
            return 1;
        }
    }
    
    printf("未找到该索引号的图书！\n");
    return 0;
}

void displayAllBooks() {
    if (bookCount == 0) {
        printf("图书库为空！\n");
        return;
    }
    
    printf("\n%-10s %-30s %-20s %-20s %-8s %-8s %-8s\n", 
           "索引号", "书名", "作者", "出版社", "单价", "库存", "借出");
    printf("--------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < bookCount; i++) {
        printf("%-10s %-30s %-20s %-20s %-8.2f %-8d %-8d\n", 
               books[i].id, books[i].name, books[i].author, books[i].publisher, 
               books[i].price, books[i].total, books[i].borrowed);
    }
    printf("\n");
}

Book* findBookById(const char* id) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].id, id) == 0) {
            return &books[i];
        }
    }
    return NULL;
}

Book* findBookByName(const char* name) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].name, name) == 0) {
            return &books[i];
        }
    }
    return NULL;
}

void saveBooks() {
    if (saveDataToFile(books, sizeof(Book), bookCount, BOOKS_FILE)) {
        printf("图书数据保存成功！\n");
    } else {
        printf("图书数据保存失败！\n");
    }
}

void loadBooks() {
    bookCount = loadDataFromFile(books, sizeof(Book), MAX_BOOKS, BOOKS_FILE);
    if (bookCount > 0) {
        printf("已加载%d本图书数据。\n", bookCount);
    } else {
        printf("没有找到图书数据或数据为空。\n");
        bookCount = 0;
    }
} 