#ifndef __CHATSERVER_H__
#define __CHATSERVER_H__

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);
    void start();

private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buffer,
                   Timestamp time);

    EventLoop *_loop;
    TcpServer _server;
};

#endif