# 一 概述
  TCP-FastOpen，顾名思义就是TCP的快速打开机制，简称TFO。
# 二 作用
  传统的TCP连接过程中，每次连接都需要经过三次握手机制才能正常的开始传输数据。
  在TFO的加持下，若客户端和服务器之间，在之前已经正常进行过三次握手。那么，后续的三次握手阶段，客户端在发送第一个SYN包的时候，就可以开始传输数据了。
  由此，便可以降低三次握手对于网络延迟的影响。
# 三 开启TCP-FastOpen
  sysctl -w net.ipv4.tcp_fastopen=3
  或者
  echo 3 > /proc/sys/net/ipv4/tcp_fastopen
# 四 参考链接
  https://zhuanlan.zhihu.com/p/612423468
