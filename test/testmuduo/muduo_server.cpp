#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>

using namespace muduo;
using namespace muduo::net;

/*基于muduo网络库开发服务器程序
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写时间的回调函数
5.设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/

class ChatServer
{
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg) : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
        _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        _server.setThreadNum(4);
    }

    void start(){
        _server.start();
    }

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            std::cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:online" << std::endl;
        }
        else
        {
            std::cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:offline" << std::endl;
            conn->shutdown(); // close(fd)
            // _loop->quit();
        }
    }

    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)
    {
        string buf = buffer->retrieveAllAsString();
        std::cout << "recv data:" << buf << " time:" << time.toFormattedString() << std::endl;
        conn->send(buf);
    }
    TcpServer _server;
    EventLoop *_loop;
};


int main(){
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop, addr, "ChatServer");
    server.start();
    loop.loop(); // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
}