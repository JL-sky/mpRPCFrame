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
    //发布rpc方法
    void notifyService(google::protobuf::Service*service);
    //启动rpc服务,使用muduo库发布网络
    void run();
private:
    muduo::net::EventLoop _loop;
    void connectionCallback(const muduo::net::TcpConnectionPtr&);
    void messageCallback(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp time);

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
    // Closure回调，用于序列化rpc的响应和网络发送
    void sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message*);
};