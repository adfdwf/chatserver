#ifndef __CHATSERVICE_H__
#define __CHATSERVICE_H__

#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include <mutex>

#include "json.hpp"
#include "usermodel.h"
#include "offlinemessagemodel.h"
#include "friendmodel.h"
#include "groupmodel.h"
#include "redis.h"

using namespace nlohmann;
using namespace muduo;
using namespace muduo::net;

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class ChatService
{
public:
    static ChatService *instance();
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //一对一聊天
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 获取消息对应的Handler
    MsgHandler getHandler(int msgid);
    void clientCloseException(const TcpConnectionPtr& conn);
    // 服务器异常，业务重置方法
    void reset();
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);

private:
    ChatService();
    std::unordered_map<int, MsgHandler> _msgHandlerMap;
    //数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;
    // 存储在线用户的通信连接
    std::unordered_map<int, TcpConnectionPtr> _userConnMap;
    // 定义互斥锁，保证_userConnMap的线程安全
    std::mutex _connMutex;
    Redis _redis;
};

#endif
