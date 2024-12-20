# 一 概述

# 二 三次握手状态变更
## 2.1 服务端
### 2.1.1 LISTEN状态
  LISTEN状态是TCP服务中的第一个状态，当服务端正常调用listen系统调用之后，便进入了LISTEN状态。
### 2.1.2 流程解析
  用户态程序调用listen函数 --> 执行listen的系统调用 --> 执行内核协议栈中的listen函数 --> 进入LISTEN状态
#### 2.1.2.1 用户态listen函数
  int listen(int sockfd, int backlog);
  参数1：sockfd -> socket()创建的socket文件描述符
  参数2：backlog -> 设置全连接队列长度(PS:不一定生效)
#### 2.1.2.2 listen系统调用
  SYSCALL_DEFINE2(listen, int, fd, int, backlog) -- [net\socket.c]
  Step1：获取全连接队列长度的设置：min(backlog,net.core.somaxconn)的最小值,其中的net.core.somaxconn是系统参数：
    sysctl -w net.core.somaxconn=xxx 或者 修改/proc/sys/net/core/somaxconn文件
#### 2.1.2.3 内核协议栈inet_listen处理函数
  若创建的套接字类型domain为AF_INET，则最终会调用到[net\ipv4\af_inet.c]中的inet_listen函数进行下一步处理。
  Step1：判断TC-FastOpen属性是否开启：sysctl net.ipv4.tcp_fastopen或者读取/proc/sys/net/ipv4/tcp_fastopen文件（默认关闭）
    PS：TCP-FastOpen属性的详细介绍，参考：《TCP-FastOpen简介.md》
  Step2：执行inet_csk_listen_start函数，开始初始化相关资源，设置状态为LISTEN状态 -- [net\ipv4\inet_connection_sock.c]
  Step3：申请和初始化接收队列，包括全连接队列（链表）和半连接队列（哈希表）

## 2.2 客户端
### 2.2.1 SYN状态
  SYN状态是客户端的第一个正常状态，当客户端调用connect系统调用向服务端发起连接请求时。
  客户端首先进入的便是SYN状态。
### 2.2.2 流程解析
  用户态程序调用connect函数 --> 执行connect的系统调用 --> 执行内核协议栈中的connect函数 --> 与服务端建立连接
#### 2.2.2.1 用户态connect
  int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
  参数1：socket创建的套接字文件描述符
  参数2：服务端的地址信息
  参数3：服务端的地址长度
#### 2.2.2.2 connect系统调用
  SYSCALL_DEFINE3(connect, int, fd, struct sockaddr __user *, uservaddr, int,addrlen) -- [net\socket.c]
#### 2.2.2.3 内核态inet_stream_connect
  若创建的套接字类型domain为AF_INET，且type为SOCK_STREAM，则最终会调用到[inet_stream_connect]中的inet_listen函数进行下一步处理。
  Step1：刚刚创建的socket的默认状态为SS_UNCONNECTED，故进入该switch...case分支进行处理
  Step2：执行TCP协议栈中的tcp_v4_connect，发起向服务端的连接请求 -- [net\ipv4\tcp_ipv4.c]
    1.tcp_set_state(sk, TCP_SYN_SENT):设置当前socket的状态为SYN_SENT状态
    2.inet_hash_connect():如果没有bind操作，就动态选择一个端口，作为源端口 -- 其中有两个主要工作：①随机选择一个端口  ②判断选择的端口是否已经被处于  ESTABLISH状态的套接字使用。
    其中，如果是动态选择一个随机端口，这个端口的范围是可配置的：sysctl net.ipv4.ip_local_port_range是或者查看/proc/sys/net/ipv4/ip_local_port_range文件：
    [net.ipv4.ip_local_port_range = 32768	60999] --> 由此可知，客户端允许选择的端口范围必须满足这个配置
    除此之外，如果不希望客户端使用某些特殊的端口，可以通过设置net.ipv4.ip_local_reserved_ports内核参数，来设置系统保留的端口信息
    当系统没有可用端口时，客户端尝试建立与服务器之间的连接时，系统便会提示：
    [Cannot assign requested address]
    3.tcp_connect():构造一个syn报文，发送给服务端
### 2.2.3 SYN重传机制
  客户端在调用上述tcp_connect()构造并向客户端发送syn报文时，还会启动一个syn重传定时器。
  inet_csk_reset_xmit_timer()


