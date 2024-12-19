#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 9090

int main(int argc, char *argvp[])
{
  // Step1：创建一个TCP协议的socket套接字(默认阻塞)，返回socket文件描述符
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    perror("Create a tcp socket failed");
    return -1; // 创建socket套接字失败
  }
  printf("Create socket success:%d\n", socket_fd);

  // Step2： bind() -- 绑定服务器的地址信息(IP和端口)，返回bind结果
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;          // IP协议簇设置为IPV4
  server_addr.sin_port = htons(SERVER_PORT); // 设置服务器监听的端口号
  server_addr.sin_addr.s_addr = INADDR_ANY;  // 设置服务器监听的IP地址（此处为任意地址）
  int ret = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < -1)
  {
    perror("Bind failed");
    return -1;
  }
  printf("Bind socket success\n");

  // Step3:listen() -- 服务器开始监听，等待客户端的连接
  /*
    Tips:当Listen成功之后，服务器将进入【LISTEN】状态（TCP状态机中的状态之一，也是服务端的第一个正常状态），使用netstat命令可以查看：
    进入【LISTEN】状态之后，服务器才允许客户端进行连接
    tcp        0      0 0.0.0.0:9090            0.0.0.0:*               LISTEN      20025/./tcp_hand_sh
  */
  ret = listen(socket_fd, 0);
  if (ret < 0)
  {
    perror("Listen failed:");
    return -1;
  }
  printf("Server start listen\n");
  /*
  Tips:当Listen成功之后，客户端便可以连接服务器了。
  也就是大名鼎鼎的“三次握手”，具体网络报文可以查看 ../pcap/hand_shank.pcap
  */

  // Step4: accept() -- 等待客户端的连接,返回客户端对应的文件描述符
  // int client_fd = accept(socket_fd, );
  getchar();
}
