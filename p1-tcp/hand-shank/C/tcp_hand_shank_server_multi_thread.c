/*********************************************头文件包含***********************************/
// 标准库头文件
#include <pthread.h>
// 自定义头文件
#include "common.h"

// 处理客户端的连接请求
void *accept_client_handler(void *arg)
{
  int client_fd = *(int *)arg; // 客户端文件描述符
  while (1)
  {
    // 接收客户端发送的数据
    char recv_buff[128] = {0};
    int recv_ret = recv(client_fd, recv_buff, sizeof(recv_buff) / sizeof(recv_buff[0]), 0);
    if (recv_ret < 0)
    {
      perror("Received data from client error:");
      break;
    }
    else if (recv_ret == 0)
    {
      // 客户端断开连接
      printf("Client disconnect\n");
      break;
    }
    else
    {
      // 接收到客户端发送的数据，并原样返回
      printf("Server recevied data from client is:%s\n", recv_buff);
      int send_ret = send(client_fd, recv_buff, recv_ret, 0);
      if (send_ret < 0)
      {
        perror("Echo send failed");
        break;
      }
      printf("Echo send data length:%d\n", send_ret);
    }
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  int close_ret = 0;
  // Step1：创建一个TCP协议的socket套接字，返回socket文件描述符
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    perror("Server create a tcp socket failed");
    return -1; // 创建socket套接字失败
  }
  printf("Server create socket success:%d\n", socket_fd);

  // Step2： bind() -- 绑定服务器的地址信息(IP和端口)，返回bind结果
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;                // IP协议簇设置为IPV4
  server_addr.sin_port = htons(SERVER_BIND_PORT);  // 设置服务器监听的端口号
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 设置服务器监听的IP地址（此处为任意地址）
  int bind_ret = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_ret < -1)
  {
    perror("Server bind failed");
    goto clean_server;
  }
  printf("Server bind socket success\n");

  // Step3:listen() -- 服务器开始监听，等待客户端的连接（进入【LISTEN】状态）
  /*
    Tips:当Listen成功之后，服务器将进入【LISTEN】状态（TCP状态机中的状态之一，也是服务端的第一个正常状态），使用netstat命令可以查看：
    进入【LISTEN】状态之后，服务器才允许客户端进行连接
    tcp        0      0 0.0.0.0:9090            0.0.0.0:*               LISTEN      20025/./tcp_hand_sh
  */

  int listen_ret = listen(socket_fd, 128);
  if (listen_ret < 0)
  {
    perror("Server listen failed:");
    goto clean_server;
  }
  printf("Server start listen\n");

  /*
  Tips:当Listen成功之后，客户端便可以连接服务器了。
  也就是大名鼎鼎的“三次握手”，具体网络报文可以查看 ../pcap/hand_shank.pcap
  */

  // Step4: accept() -- 等待客户端的连接,返回客户端对应的文件描述符
  pthread_t pid = 0;
  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(struct sockaddr_in);
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
      perror("接收客户端的连接请求失败:");
      continue;
    }
    printf("接收到客户端的连接请求，IP地址：%x  端口号：%d\n", client_addr.sin_addr.s_addr, ntohs(client_addr.sin_port));
    // 创建单独的线程，处理客户端的连接
    int ret = pthread_create(&pid, NULL, accept_client_handler, &client_fd);
    if (ret < 0)
    {
      perror("Create pthread failed");
      continue;
    }
  }

  pthread_join(pid, NULL); // 等待所有线程的结束
clean_server:
  close_ret = close(socket_fd);
  if (close_ret < 0)
  {
    perror("Server disconnect server failed:");
    return -1;
  }
  printf("Server disconnect server success!\n");
}
