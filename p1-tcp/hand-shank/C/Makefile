# 指定编译器
CC = gcc
 
# 编译选项
CFLAGS = -Wall -g
 
# 源文件列表
SERVER_SRCS = tcp_hand_shank_server.c
CLIENT_SRCS = tcp_hand_shank_client.c
 
# 生成的目标文件列表（.o 文件）
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
 
# 最终生成的可执行文件
SERVER_TARGET = tcp_hand_shank_server
CLIENT_TARGET = tcp_hand_shank_client
 
# 默认目标
all: $(SERVER_TARGET) $(CLIENT_TARGET)
 
# 链接目标文件生成可执行文件
# 编译server
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $(SERVER_TARGET) -lpthread
# 编译client
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $(CLIENT_TARGET)
 
# 编译源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
 
# 清理生成的文件
clean:
	rm -f $(SERVER_OBJS) $(SERVER_TARGET)
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)
	rm -rf *.o
 
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