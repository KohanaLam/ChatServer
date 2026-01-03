#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;  //参数占位符

//基于muduo网络库开发服务器程序
//1.组合TcpServer对象
//2.创建EventLoop事件循环对象指针
//3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
//4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
//5.设置合适的服务端线程数量，muduo库会自己分配监听线程和工作线程

class ChatServer
{
public:
    ChatServer(EventLoop *loop,   //事件循环
                const InetAddress &listenAddr,  //IP+port
                const string &nameArg)  //服务器名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
        {
            // 给服务器注册用户连接的创建和断开回调
            //因为下面这个setConnectionCallback函数定义只有一个参数，而这个onConnection是成员函数，隐含this指针
            //所以要bind函数，_1是参数占位符，表示有一个参数
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));


            // 给服务器注册用户读写事件回调
            _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
            
            // 设置服务器端的线程数量  1个监听I/O线程   1个worker线程
            _server.setThreadNum(4);
        }

        // 开启事件循环
        void start()
        
        {
            _server.start();
        }

private:
    // 专门处理用户的连接创建和断开   epoll  listenfd  accept一系列的封装，留下这一个接口
    void onConnection(const TcpConnectionPtr &conn)
    {
        if(conn->connected())  //这个函数返回一个bool值，表示连接是否连上
        {
            cout<< conn->peerAddress().toIpPort() << " -> " <<conn->localAddress().toIpPort()<< "state:online"<< endl; //打印对端信息和自己的信息（IP地址和端口号）
        }
        else
        {
            cout<< conn->peerAddress().toIpPort() << " -> " <<conn->localAddress().toIpPort()<< "state:offline"<< endl;
            conn->shutdown(); //相当于close(fd)
            // _loop->quit(); //相当于退出循环了，连接断开
        }
    }

    //专门处理用户读写事件
    void onMessage(const TcpConnectionPtr &conn,  //连接
                    Buffer *buffer,  //缓冲区
                    Timestamp time) //接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString(); //buffer是muduo库封装的缓存类，这里将他转为字符串
        cout<<"recv data"<<buf<<"time: "<<time.toString()<<endl;
        conn->send(buf); //发回去，相当于echo服务器
    }

    TcpServer _server; //1
    EventLoop *_loop; //2 看作epoll
};


int main()
{
    EventLoop loop;  //相当于创建epoll
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // 想当于listenfd epoll_ctl->epoll
    loop.loop(); //开始循环等待事件，相当于epoll_wait以阻塞的方式等待新用户连接，已连接用户的读写事件等

    return 0;
}