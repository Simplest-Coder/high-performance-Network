#ifndef _TCP_HAND_SHANK_COMMON_H_
#define _TCP_HAND_SHANK_COMMON_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_BIND_PORT 9090             // 服务器监听的端口信息
#define SERVER_BIND_IPADDR "10.34.16.186" // 服务器监听的IP地址信息
#define MAX_BUFFER_LENGTH 1024            // 用户态接收/发送缓冲区大小

#define ENABLE_CLIENT_BIND 0
#if ENABLE_CLIENT_BIND
#define CLIENT_BIND_PORT 8080 // 客户端可选的端口绑定
#endif

#endif /* _TCP_HAND_SHANK_COMMON_H_ */