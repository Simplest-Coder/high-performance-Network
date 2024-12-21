/*********************************************头文件包含***********************************/
// 标准库头文件

// 自定义头文件
#include "common.h"

int main(int argc, char *argvp[])
{
  int close_ret = 0;
  // Step1：创建一个TCP协议的socket套接字，返回socket文件描述符
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    perror("Client create a tcp socket failed");
    return -1; // 创建socket套接字失败
  }
  printf("Client create socket success:%d\n", socket_fd);

  // Step2：bind() -- 可选操作(可以指定客户端使用的端口号，如果不进行bind操作，那么客户端使用的就是随机端口)
  /*
    客户端绑定端口信息之后，便会使用固定的端口号，连接Server，如下所示：
    其中的Server端监听的端口号为9090，客户端使用的端口号固定为8080
    tcp        0      0 10.34.16.186:8080       10.34.16.186:9090       ESTABLISHED 24154/./tcp_hand_sh
    tcp        0      0 10.34.16.186:9090       10.34.16.186:8080       ESTABLISHED -
  */
#if ENABLE_CLIENT_BIND
  struct sockaddr_in client_addr;
  client_addr.sin_family = AF_INET;               // IP协议簇设置为IPV4
  client_addr.sin_port = htons(CLIENT_BIND_PORT); // 设置客户端使用的端口号
  client_addr.sin_addr.s_addr = INADDR_ANY;       // 设置客户端使用的的IP地址
  int bind_ret = bind(socket_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
  if (bind_ret < -1)
  {
    perror("Client bind failed");
    goto clean;
  }
  printf("Client bind socket success\n");
#endif

  // Step3:connect() --- 连接服务器
  /*
    在这个过程中，客户端在与服务器正常建立连接的过程中，会经历下述状态的变更：
    part1:[SYN_SENT]状态
      tcp        0      1 10.34.16.186:57888      10.34.16.186:9090       SYN_SENT    33213/./tcp_hand_sh
    part2:[ESTABLISHED]状态
      tcp        0      0 10.34.16.186:9090       10.34.16.186:47188      ESTABLISHED -
   */
  // 设置Server端的地址信息
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;                            // IP协议簇设置为IPV4
  server_addr.sin_port = htons(SERVER_BIND_PORT);              // 连接服务器监听的端口号
  server_addr.sin_addr.s_addr = inet_addr(SERVER_BIND_IPADDR); // 连接服务器监听的IP地址
  int conn_ret = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (conn_ret < 0)
  {
    perror("Client connect server failed:");
    goto clean;
  }
  printf("Client connect server success!\n");
  getchar();
clean:
  // Step4:客户端主动断开连接，进入CLOSE_WAIT状态
  close_ret = close(socket_fd);
  if (close_ret < 0)
  {
    perror("Client disconnect server failed:");
    return -1;
  }
  printf("Client disconnect server success!\n");
}
