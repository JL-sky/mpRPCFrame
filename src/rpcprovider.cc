#include "rpcprovider.h"
#include<string>
#include "rpcheader.pb.h"
#include "zkutil.h"

//获取远程注册的对象信息（远程调用对象，远程调用对象的信息）
void RpcProvider::notifyService(google::protobuf::Service*service)
{
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc=service->GetDescriptor();
    // 获取服务名称
    std::string serviceName=pserviceDesc->name();
    LOG_INFO("service_name:%s",serviceName.c_str());
    _serviceInfo serviceInfo;
    serviceInfo._service=service;

    // 获取服务中方法的数量
    const int methodCnt=pserviceDesc->method_count();
    for(int i=0;i<methodCnt;i++)
    {
        //获取指定服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc=pserviceDesc->method(i);
        std::string methodName=pmethodDesc->name();
        serviceInfo._methodMap.insert({methodName,pmethodDesc});
        LOG_INFO("method_name:%s",methodName.c_str());
    }
    _serviceMap.insert({serviceName,serviceInfo});
}

// rpc网络模块
void RpcProvider::run()
{
    //封装服务端地址
    std::string ip=MpRpcApplication::getInstance().getConfig().load("rpcServerIp");
    uint16_t port=atoi(MpRpcApplication::getInstance().getConfig().load("rpcServerPort").c_str());
    muduo::net::InetAddress rpcProInetAdd(ip,port);
    //创建muduo服务器对象
    muduo::net::TcpServer server(&_loop,rpcProInetAdd,"rpcProvider");
    
    //设置网络连接建立事件回调
    server.setConnectionCallback(std::bind(
        &RpcProvider::connectionCallback,this,
        std::placeholders::_1));
    //设置网络连接读写事件回调
    server.setMessageCallback(std::bind(
        &RpcProvider::messageCallback,this,
        std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    // std::cout<<"rpcProvier server start!"<<std::endl;
    LOG_INFO("rpcProvier server start!");
    //一个IO线程，3个工作线程
    server.setThreadNum(4);

    // 将发布的rpc服务注册到zk上，从而使得rpc客户端可以在zk上发现服务
    ZkClient zkc;
    zkc.start();
    for(auto &sp:_serviceMap)
    {
        std::string serviceName=sp.first;
        std::string servicePath="/"+serviceName;
        // 创建永久性对象节点，即使节点挂掉也不会被删除
        zkc.createNode(servicePath.c_str(),nullptr,0);
        for(auto &mp:sp.second._methodMap)
        {
            std::string methodPath="/"+serviceName+"/"+mp.first;
            char databuffer[128]={0};
            sprintf(databuffer,"%s:%d",ip.c_str(),port);
            // ZOO_EPHEMERAL表示方法节点是一个临时节点
            zkc.createNode(methodPath.c_str(),databuffer,strlen(databuffer),ZOO_EPHEMERAL);
        }
    }


    server.start();
    _loop.loop();
}

void RpcProvider::connectionCallback(const muduo::net::TcpConnectionPtr& conn)
{
    if(!conn->connected())
    {
        conn->shutdown();
    }
}

/*
网络数据：
headerSize(4B存储)+headerStr(serviceName+methodName+argsSize)+argsStr
*/
void RpcProvider::messageCallback(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp time)
{
    /*从字符流中抽取远程服务及其方法名和参数信息*/
    std::string recv_buf=buffer->retrieveAllAsString();
    // 从字符流读取前4个字节的数据，即headerSize
    uint32_t headerSize=0;
    recv_buf.copy((char*)&headerSize,4,0);

    // 读取网络字节头部信息
    std::string headerStr=recv_buf.substr(4,headerSize);
    mprpcHeader::RpcHeader rpcHeader;
    // 字符串反序列化
    std::string serviceName;
    std::string methodName;
    int argsSize;
    if(rpcHeader.ParseFromString(headerStr))
    {
        serviceName=rpcHeader.servicename();
        methodName=rpcHeader.methodname();
        argsSize=rpcHeader.argssize();
    }
    else//反序列化失败
    {
        // std::cout<<"headerStr:"<<headerStr<<std::endl;
        LOG_ERR("headerStr:%s",headerStr.c_str());
        return ;
    }

    std::string args=recv_buf.substr(4+headerSize,argsSize);
    
    std::cout<<"============================"<<std::endl;
    std::cout<<"headerSize:"<<headerSize<<std::endl;
    std::cout<<"headerStr:"<<headerStr<<std::endl;
    std::cout<<"serviceName:"<<serviceName<<std::endl;
    std::cout<<"methodName:"<<methodName<<std::endl;
    std::cout<<"argsSize:"<<argsSize<<std::endl;
    std::cout<<"args:"<<args<<std::endl;
    std::cout<<"============================"<<std::endl;


    // 获取服务对象和方法对象
    auto mit=_serviceMap.find(serviceName);
    if(mit==_serviceMap.end())
    {
        // std::cout<<serviceName<<" is not exist!"<<std::endl;
        LOG_ERR("%s is not exist!",serviceName.c_str());
        return ;
    }

    auto it=mit->second._methodMap.find(methodName);
    if(it==mit->second._methodMap.end())
    {
        // std::cout<<serviceName<<":"<<methodName<<" is not exist!"<<std::endl;
        LOG_ERR("%s:%s is not exist!",serviceName.c_str(),methodName.c_str());
        return;
    }

    // 获取服务对象
    google::protobuf::Service* pService=mit->second._service;
    // 获取服务对应下的方法对象
    const google::protobuf::MethodDescriptor* pMethod=it->second;
    
    // 生成rpc调用需要的request参与和response参数
    /*
    void login(::google::protobuf::RpcController* controller,
                       const ::mpRpc::LoginRequest* request,
                       ::mpRpc::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    */
    // 获取远程请求对象
    google::protobuf::Message *request=pService->GetRequestPrototype(pMethod).New();
    // 解析远程请求参数
    if(!request->ParseFromString(args))
    {
        // std::cout<<"request args parse error!"<<std::endl;
        LOG_ERR("request args parse error!");
        return;
    }
    google::protobuf::Message *response=pService->GetResponsePrototype(pMethod).New();
    //设置rpc响应调用的方法回调
    /*
    template <typename Class, typename Arg1, typename Arg2>
    inline Closure* NewCallback(Class* object, void (Class::*method)(Arg1, Arg2),
                                Arg1 arg1, Arg2 arg2)
    */
    google::protobuf::Closure *done=google::protobuf::NewCallback<RpcProvider,
                                const muduo::net::TcpConnectionPtr&,
                                google::protobuf::Message*>
                                            (this,
                                            &RpcProvider::sendRpcResponse,
                                            conn,
                                            response);
    // rpc远程调用
    pService->CallMethod(pMethod,nullptr,request,response,done);
}

// Closure回调，用于序列化rpc的响应和网络发送
void RpcProvider::sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,
                                    google::protobuf::Message* response)
{
    std::string responseStr;
    if(response->SerializeToString(&responseStr))//响应数据序列化
    {
        conn->send(responseStr);
    }
    else{
        // std::cout<<"response str serialize error!"<<std::endl;
        LOG_ERR("response str serialize error!");
    }
    conn->shutdown();
}