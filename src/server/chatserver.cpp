#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json; //起别名

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop* loop,
            const InetAddress& listenAddr, const string& nameArg)
            : _server(loop, listenAddr, nameArg), _loop(loop)
            {
                // 注册连接回调
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

                // 注册消息回调
                _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

                // 设置线程数量
                _server.setThreadNum(4);
            }

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 上报连接相关信息的回调函数    
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开连接
    if(!conn->connected())
    {
        //因为客户端异常关闭(断开连接)的时候应该要设置他的状态为下线
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }

}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"]一个id回调一个东西 获取-> 业务handler-> conn js time，而不是通过switch case这样跳转到对应的登录，注册等方法，这样是强耦合
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>()); // 第二个括号里面是把js里面的这个数字转成int
    // 回调消息绑定好的事件处理器，来执行相应的业务处理，也就是在这里调用相应的业务函数
    msgHandler(conn, js, time);
}


