#include "usermodel.hpp"
#include "db.h"
#include <iostream>

// User表的增加方法
bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')", 
        user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id，因为数据库里id是自增的，但是外面的user是不知道id的，所以需要自己设置
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }

    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);
    
    MySQL mysql;
    if(mysql.connect())
    {
        // 表示「查询执行成功且返回了结果集容器」,不一定有数据，简单来说就是查询没报错
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr) // 查到了
        {
            MYSQL_ROW row = mysql_fetch_row(res); // 因为肯定只查到一行，因为id是主键，只对应一个人
            // 表示「结果集中有实际的数据行」
            if(row != nullptr)
            {
                User user;
                // 0 1 2 3刚好对应那个user表的四个属性
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res); // 释放资源
                return user;
            }
        }
        
    }

    return User(); // 没找到，返回默认构造的
}

// 更新用户的状态信息
bool UserModel::updateState(User user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId());
    
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
    }

    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update user set state = 'offline' where state = 'online'";
    
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}