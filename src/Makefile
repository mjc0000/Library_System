# Makefile for 图书馆借阅系统

# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -g

# 源文件
SRCS = src/main.c src/book.c src/student.c src/borrow.c src/fileio.c src/auth.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# 可执行文件名称
TARGET = library_system

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# 编译源文件为目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: all clean
