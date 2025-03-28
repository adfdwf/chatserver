#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/*
server和client的公共文件
*/

enum EnMsgtype
{
    LOGIN_MSG = 1,
    LOGIN_MSG_ACK,
    LOGINOUT_MSG,
    REG_MSG,
    REG_MSG_ACK, // 注册相应消息
    ONE_CHAT_MSG,
    ADD_FRIEND_MSG,
    
    CREATE_GROUP_MSG, // 创建群组
    ADD_GROUP_MSG,    // 加入群组
    GROUP_CHAT_MSG,   // 群聊天
};

#endif
