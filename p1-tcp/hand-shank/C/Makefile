# 指定编译器
CC = gcc
 
# 编译选项
CFLAGS = -Wall -g
 
# 源文件列表
SRCS = tcp_hand_shank.c
 
# 生成的目标文件列表（.o 文件）
OBJS = $(SRCS:.c=.o)
 
# 最终生成的可执行文件
TARGET = tcp_hand_shank
 
# 默认目标
all: $(TARGET)
 
# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
 
# 编译源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
 
# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
 
# 显示编译命令（可选）
.PHONY: VERBOSE
VERBOSE:
	@make -n
 
# 打印帮助信息（可选）
.PHONY: help
help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all       - Build the program"
	@echo "  clean     - Clean up generated files"
	@echo "  VERBOSE   - Display the commands without executing them"
	@echo "  help      - Display this help message"