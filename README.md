# ChatServer
集群聊天服务器和客户端代码，包含nginx tcp负载均衡功能，基于muduo库实现， redis mysql

编译方式
cd build
rm -rf *
cmake ..
make

运行：
需要nginx tcp负载均衡
