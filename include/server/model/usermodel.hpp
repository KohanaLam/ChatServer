#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"

// user这张表的数据操作类
class UserModel
{
public:
    // User表的增加方法（增加用户）
    bool insert(User &user);

    // 根据用户号码查询用户信息 (感觉不应该用id，因为实际上用户并不会记住自己的id，然后输入查询)
    User query(int id);

    // 更新用户的状态信息
    bool updateState(User user);

    // 重置用户的状态信息
    void resetState();
};



#endif