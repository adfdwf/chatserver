#include "redis.h"
#include <string>
#include <iostream>
#include <unistd.h>

Redis::Redis()
    : _publish_context(nullptr), _subcribe_context(nullptr), _notify_message_handler(nullptr)
{
}

Redis::~Redis()
{
    // 断开连接并释放资源
    if (_publish_context != nullptr)
    {
        redisFree(_publish_context);
        _publish_context = nullptr;
    }
    if (_subcribe_context != nullptr)
    {
        redisFree(_subcribe_context);
        _subcribe_context = nullptr;
    }
}

bool Redis::connect()
{
    // 连接redis服务器，用于发布消息的上下文
    _publish_context = redisConnect("127.0.0.1", 6379);
    if (_publish_context == nullptr || _publish_context->err)
    {
        if (_publish_context)
        {
            cerr << "Connect to redis failed: " << _publish_context->errstr << endl;
        }
        else
        {
            cerr << "Can't allocate redis context" << endl;
        }
        return false;
    }

    // 连接redis服务器，用于订阅消息的上下文
    _subcribe_context = redisConnect("127.0.0.1", 6379);
    if (_subcribe_context == nullptr || _subcribe_context->err)
    {
        if (_subcribe_context)
        {
            cerr << "Connect to redis failed: " << _subcribe_context->errstr << endl;
        }
        else
        {
            cerr << "Can't allocate redis context" << endl;
        }
        return false;
    }
    
    // 在单独的线程中，监听通道上的事件，有消息给业务层进行上报
    thread t([&]()
             { observer_channel_message(); });
    t.detach();

    return true;
}

bool Redis::publish(int channel, string message)
{
    if (_publish_context == nullptr)
    {
        cerr << "publish context is null" << endl;
        return false;
    }

    // 使用redisCommand向指定通道发布消息
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message.c_str());
    if (reply == nullptr)
    {
        cerr << "publish command failed" << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool Redis::subscribe(int channel)
{
    if (_subcribe_context == nullptr)
    {
        cerr << "subcribe context is null" << endl;
        return false;
    }

    // 只负责发送订阅命令，不阻塞等待响应
    if (REDIS_ERR == redisAppendCommand(_subcribe_context, "SUBSCRIBE %d", channel))
    {
        cerr << "subscribe command failed" << endl;
        return false;
    }

    // 循环发送缓冲区中的命令，直到发送完毕
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(_subcribe_context, &done))
        {
            cerr << "subscribe command write failed" << endl;
            return false;
        }
    }

    return true;
}

bool Redis::unsubscribe(int channel)
{
    if (_subcribe_context == nullptr)
    {
        cerr << "subcribe context is null" << endl;
        return false;
    }

    // 发送取消订阅命令
    if (REDIS_ERR == redisAppendCommand(_subcribe_context, "UNSUBSCRIBE %d", channel))
    {
        cerr << "unsubscribe command failed" << endl;
        return false;
    }

    // 循环发送缓冲区中的命令，直到发送完毕
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(_subcribe_context, &done))
        {
            cerr << "unsubscribe command write failed" << endl;
            return false;
        }
    }

    return true;
}

// 在独立线程中接收订阅通道中的消息
void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->_subcribe_context, (void **)&reply))
    {
        // 订阅收到的消息是一个带三元素的数组
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            // 给业务层上报通道上发生的消息
            _notify_message_handler(atoi(reply->element[1]->str), reply->element[2]->str);
        }

        freeReplyObject(reply);
    }

    cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << endl;
}

void Redis::init_notify_handler(function<void(int, string)> fn)
{
    _notify_message_handler = fn;
}