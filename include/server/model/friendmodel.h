#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
#include <vector>
#include "user.h"
// 维护好友信息的操作接口方法
class FriendModel
{
public:
    // 添加好友关系
    void insert(int userid, int friendid);

    // 返回用户好友列表
    std::vector<User> query(int userid);
};

#endif