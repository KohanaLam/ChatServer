#include "json.hpp"
using json = nlohmann::json;

#include<iostream>
#include<vector>
#include<map>
using namespace std;

//json序列化示例
string func1()
{
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello world";

    string sendBuf = js.dump();
    return sendBuf;
    //cout<<sendBuf<<endl;
}

string func2()
{
    json js;
    // 添加数组
    js["id"] = {1,2,3,4,5}; 
    // 添加key-value
    js["name"] = "zhang san"; 
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china"; 
    // 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    //cout << js << endl;
    return js.dump();
}

string func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    string sendBuf = js.dump(); //json数据对象->序列化json字符串
    //cout<<js<<endl;
    return sendBuf;
}

int main()
{
    string recvBuf = func3();
    // 数据的反序列化   json字符串 => 反序列化 json数据对象（可以直接看做是容器，方便访问）
    cout<<recvBuf<<endl;
    json jsbuf = json::parse(recvBuf); 
    // cout<<jsbuf["msg_type"]<<endl;
    // cout<<jsbuf["from"]<<endl;
    // cout<<jsbuf["to"]<<endl;
    // cout<<jsbuf["msg"]<<endl;
    
    // cout<<jsbuf["id"]<<endl;
    // for(auto x : jsbuf["id"])
    //     cout<<x<<" ";
    // cout<<endl;

    // auto msgjs = jsbuf["msg"];
    // cout<<msgjs["zhang san"]<<endl;
    
    vector<int> vec = jsbuf["list"]; // js对象里面的数组类型，直接放入vector容器当中

    for(auto &x : vec)
        cout<<x<<" ";
    cout<<endl;
    
    map<int, string> mymap = jsbuf["path"];

    for(auto &x : mymap)
    {
        cout<<x.first<<x.second<<endl;
    }



    
    return 0;
}
