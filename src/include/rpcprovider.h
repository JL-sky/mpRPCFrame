#pragma once
#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include "mprpcapplication.h"
#include "logger.h"
#include<unordered_map>

//框架提供的专门提供给外部使用的，可以发布rpc方法的网络对象类
class RpcProvider
{
public:
    //在服务端注册（存储）rpc方法到_serviceMap
    void notifyService(google::protobuf::Service*service);
    //启动rpc服务,使用muduo库发布网络
    void run();
private:
    // 检测到连接或者断开事件时要做的事情
    void connectionCallback(const muduo::net::TcpConnectionPtr& conn);
    /*
    用户数据处理
    参数：
        conn：tcp连接对象
        buffer：缓冲区数据
        time：接收到数据的时间信息  
    */
    void messageCallback(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp time
                        );
    // Closure回调，服务端处理完客户端的请求后的回调函数（将响应结果回传给客户端）
    void sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message*);

    muduo::net::EventLoop _loop;
    //服务类型信息
    typedef struct _serviceInfo
    {
        //保存服务对象
        google::protobuf::Service* _service;
        //保存服务方法
        unordered_map<std::string,const google::protobuf::MethodDescriptor*> _methodMap;
    }_serviceInfo;

    //存储注册成功的服务对象和其服务方法的信息
    unordered_map<std::string,_serviceInfo> _serviceMap;
};